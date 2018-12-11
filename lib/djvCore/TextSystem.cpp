//------------------------------------------------------------------------------
// Copyright (c) 2018 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

#include <djvCore/TextSystem.h>

#include <djvCore/Context.h>
#include <djvCore/FileInfo.h>
#include <djvCore/FileIO.h>
#include <djvCore/OS.h>
#include <djvCore/ResourceSystem.h>
#include <djvCore/Timer.h>

#include <condition_variable>
#include <future>
#include <locale>
#include <set>

//#pragma optimize("", off)

namespace djv
{
    namespace Core
    {
        struct TextSystem::Private
        {
            std::vector<std::string> locales;
            std::string currentLocale;
            bool currentLocaleChanged = false;
            std::condition_variable currentLocaleChangedCV;
            std::shared_ptr<ValueSubject<std::string> > currentLocaleSubject;

            std::map<std::string, std::map<std::string, std::string> > text;

            std::thread thread;
            std::mutex mutex;
            std::shared_ptr<Timer> timer;
        };

        namespace
        {
            std::string parseLocale(const std::string & value)
            {
                std::string out;
#if defined(DJV_PLATFORM_WINDOWS)
                //! \todo [1.0 M] Windows locale.
#elif defined(DJV_PLATFORM_OSX)
                //! \todo [1.0 M] OS X locale.
#else
                auto pieces = String::split(value, '_');
                if (!pieces.size())
                {
                    pieces = String::split(value, '.');
                }
                if (pieces.size())
                {
                    out = pieces[0];
                }
#endif // DJV_PLATFORM_WINDOWS
                return out;
            }
        } // namespace

        void TextSystem::_init(const std::shared_ptr<Context> & context)
        {
            ISystem::_init("djv::Core::TextSystem", context);

            _p->currentLocaleSubject = ValueSubject<std::string>::create();
            
            auto resourceSystem = context->getSystemT<ResourceSystem>();
            const auto path = resourceSystem->getPath(ResourcePath::TextDirectory);
            _p->thread = std::thread(
                [this, path]
            {
                DJV_PRIVATE_PTR();
                std::unique_lock<std::mutex> lock(p.mutex);

                p.currentLocale = "en";
                p.currentLocaleChanged = true;
                std::string djvLocale = OS::getEnv("DJV_LANG");
                std::stringstream s;
                if (djvLocale.size())
                {
                    p.currentLocale = djvLocale;
                }
                else
                {
                    try
                    {
                        std::locale locale("");
                        s << "Current std::locale: " << locale.name();
                        _log(s.str());
                        std::string cppLocale = parseLocale(locale.name());
                        if (cppLocale.size())
                        {
                            p.currentLocale = cppLocale;
                        }
                    }
                    catch (const std::exception & e)
                    {
                        _log(e.what(), LogLevel::Error);
                    }
                }
                s.str(std::string());
                s << "Current locale: " << p.currentLocale;
                _log(s.str());

                // Find the .text files.
                DirListOptions options;
                options.glob = "*.text";
                const auto fileInfos = FileInfo::dirList(path, options);

                // Extract the locale names.
                std::set<std::string> localeSet;
                for (const auto & fileInfo : fileInfos)
                {
                    std::string temp = Path(fileInfo.getPath().getBaseName()).getExtension();
                    if (temp.size() && '.' == temp[0])
                    {
                        temp.erase(temp.begin());
                    }
                    if (temp != "all")
                    {
                        localeSet.insert(temp);
                    }
                }
                for (const auto & locale : localeSet)
                {
                    p.locales.push_back(locale);
                }
                s.str(std::string());
                s << "Found locales: " << String::join(p.locales, ", ");
                _log(s.str());

                // Read the .text files.
                _readText(path);
            });

            _p->timer = Timer::create(context);
            _p->timer->setRepeating(true);
            _p->timer->start(
                Timer::getMilliseconds(Timer::Value::Medium),
                [this](float)
            {
                DJV_PRIVATE_PTR();
                bool currentLocaleChanged = false;
                {
                    std::unique_lock<std::mutex> lock(p.mutex);
                    if (p.currentLocaleChangedCV.wait_for(
                        lock,
                        std::chrono::milliseconds(0),
                        [this]
                    {
                        return _p->currentLocaleChanged;
                    }))
                    {
                        currentLocaleChanged = true;
                        p.currentLocaleChanged = false;
                    }
                }
                if (currentLocaleChanged)
                {
                    p.currentLocaleSubject->setAlways(p.currentLocale);
                }
            });
        }

        TextSystem::TextSystem() :
            _p(new Private)
        {}

        TextSystem::~TextSystem()
        {
            if (_p->thread.joinable())
            {
                _p->thread.join();
            }
        }

        std::shared_ptr<TextSystem> TextSystem::create(const std::shared_ptr<Context> & context)
        {
            auto out = std::shared_ptr<TextSystem>(new TextSystem);
            out->_init(context);
            return out;
        }

        const std::vector<std::string> & TextSystem::getLocales() const
        {
            DJV_PRIVATE_PTR();
            std::unique_lock<std::mutex> lock(p.mutex);
            return p.locales;
        }

        const std::string & TextSystem::getCurrentLocale() const
        {
            DJV_PRIVATE_PTR();
            std::unique_lock<std::mutex> lock(p.mutex);
            return p.currentLocale;
        }

        std::shared_ptr<IValueSubject<std::string> > TextSystem::getCurrentLocaleSubject() const
        {
            return _p->currentLocaleSubject;
        }

        void TextSystem::setCurrentLocale(const std::string & value)
        {
            DJV_PRIVATE_PTR();
            std::unique_lock<std::mutex> lock(p.mutex);
            p.currentLocale = value;
            p.currentLocaleChanged = true;
        }
        
        const std::string & TextSystem::getText(const std::string & id) const
        {
            DJV_PRIVATE_PTR();
            std::unique_lock<std::mutex> lock(p.mutex);
            const auto i = p.text.find(p.currentLocale);
            if (i != p.text.end())
            {
                const auto j = i->second.find(id);
                if (j != i->second.end())
                {
                    return j->second;
                }
            }
            return id;
        }

        void TextSystem::_readText(const Path & path)
        {
            DJV_PRIVATE_PTR();
            p.text.clear();
            _log("Reading text files:");
            DirListOptions options;
            options.glob = "*.text";
            for (const auto & i : FileInfo::dirList(path, options))
            {
                _log(String::indent(1) + i.getPath().get());
                try
                {
                    const auto & path = i.getPath();
                    const auto & baseName = path.getBaseName();
                    std::string locale;
                    for (auto i = baseName.rbegin(); i != baseName.rend() && *i != '.'; ++i)
                    {
                        locale.insert(locale.begin(), *i);
                    }

                    FileIO fileIO;
                    fileIO.open(path, FileIO::Mode::Read);
                    const char* mmapP = reinterpret_cast<const char*>(fileIO.mmapP());
                    const char* mmapEnd = reinterpret_cast<const char*>(fileIO.mmapEnd());

                    // Parse the JSON.
                    picojson::value v;
                    std::string error;
                    picojson::parse(v, mmapP, mmapEnd, &error);
                    if (!error.empty())
                    {
                        std::stringstream s;
                        s << "Error reading text file: " << path << ": " << error;
                        throw std::runtime_error(s.str());
                    }

                    if (v.is<picojson::array>())
                    {
                        for (const auto & item : v.get<picojson::array>())
                        {
                            if (item.is<picojson::object>())
                            {
                                std::string id;
                                std::string text;
                                std::string description;
                                const auto & obj = item.get<picojson::object>();
                                for (auto i = obj.begin(); i != obj.end(); ++i)
                                {
                                    if ("id" == i->first)
                                    {
                                        id = i->second.to_str();
                                    }
                                    else if ("text" == i->first)
                                    {
                                        text = i->second.to_str();
                                    }
                                    else if ("description" == i->first)
                                    {
                                        description = i->second.to_str();
                                    }
                                }
                                if (!id.empty())
                                {
                                    p.text[locale][id] = text;
                                }
                            }
                        }
                    }
                }
                catch (const std::exception & e)
                {
                    _log(e.what(), LogLevel::Error);
                }
            }
        }

    } // namespace Core
} // namespace djv


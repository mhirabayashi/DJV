// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#include <djvUI/EventSystem.h>

#include <djvUI/Style.h>
#include <djvUI/UISystem.h>
#include <djvUI/Window.h>

#include <djvAV/AVSystem.h>

#include <djvCore/Context.h>
#include <djvCore/Timer.h>

//#pragma optimize("", off)

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        struct EventSystem::Private
        {
            std::shared_ptr<UI::UISystem> uiSystem;
            std::vector<std::weak_ptr<Window> > windows;
            bool textLCDRenderingDirty = false;
            std::shared_ptr<ValueObserver<bool> > textLCDRenderingObserver;
            std::shared_ptr<Time::Timer> statsTimer;
        };

        namespace
        {
            /*void getClassNames(const std::shared_ptr<IObject>& object, std::map<std::string, size_t>& out)
            {
                const std::string& className = object->getClassName();
                const auto i = out.find(className);
                if (i == out.end())
                {
                    out[className] = 0;
                }
                out[className]++;
                for (const auto& j : object->getChildren())
                {
                    getClassNames(j, out);
                }
            }*/
        
        } // namespace
        
        void EventSystem::_init(const std::string& name, const std::shared_ptr<Core::Context>& context)
        {
            IEventSystem::_init(name, context);

            DJV_PRIVATE_PTR();
            p.uiSystem = context->getSystemT<UI::UISystem>();
            addDependency(p.uiSystem);

            auto avSystem = context->getSystemT<AV::AVSystem>();
            auto weak = std::weak_ptr<EventSystem>(std::dynamic_pointer_cast<EventSystem>(shared_from_this()));
            p.textLCDRenderingObserver = ValueObserver<bool>::create(
                avSystem->observeTextLCDRendering(),
                [weak](bool value)
            {
                if (auto system = weak.lock())
                {
                    system->_p->textLCDRenderingDirty = true;
                }
            });

            p.statsTimer = Time::Timer::create(context);
            p.statsTimer->setRepeating(true);
            p.statsTimer->start(
                Time::getTime(Time::TimerValue::VerySlow),
                [weak](const std::chrono::steady_clock::time_point&, const Time::Duration&)
            {
                if (auto system = weak.lock())
                {
                    std::stringstream ss;
                    ss << "Global widget count: " << Widget::getGlobalWidgetCount();
                    system->_log(ss.str());
                    
                    /*std::map<std::string, size_t> classNames;
                    getClassNames(system->getRootObject(), classNames);
                    for (const auto& i : classNames)
                    {
                        std::stringstream ss;
                        ss << i.first << " count: " << i.second;
                        system->_log(ss.str());
                    }*/
                }
            });
        }

        EventSystem::EventSystem() :
            _p(new Private)
        {}

        EventSystem::~EventSystem()
        {}

        void EventSystem::tick()
        {
            IEventSystem::tick();
            DJV_PRIVATE_PTR();
            if (auto context = getContext().lock())
            {
                const auto& style = p.uiSystem->getStyle();
                bool redraw = style->isPaletteDirty();
                bool resize = style->isSizeDirty();
                bool font = p.textLCDRenderingDirty || style->isFontDirty();
                p.textLCDRenderingDirty = false;
                if (redraw || resize || font)
                {
                    Event::InitData data;
                    data.redraw = redraw;
                    data.resize = resize;
                    data.font = font;
                    Event::Init initEvent(data);
                    for (auto i : p.windows)
                    {
                        if (auto window = i.lock())
                        {
                            _initRecursive(window, initEvent);
                        }
                    }
                    style->setClean();
                }
                auto i = p.windows.begin();
                while (i != p.windows.end())
                {
                    bool erase = false;
                    if (auto window = i->lock())
                    {
                        if (window->isClosed())
                        {
                            getRootObject()->removeChild(window);
                            erase = true;
                        }
                    }
                    else
                    {
                        erase = true;
                    }
                    if (erase)
                    {
                        i = p.windows.erase(i);
                    }
                    else
                    {
                        ++i;
                    }
                }
            }
        }

        void EventSystem::_pushClipRect(const Core::BBox2f&)
        {
            // Default implementation does nothing.
        }

        void EventSystem::_popClipRect()
        {
            // Default implementation does nothing.
        }

        bool EventSystem::_resizeRequest(const std::shared_ptr<Widget>& widget) const
        {
            bool out = widget->_resizeRequest;
            widget->_resizeRequest = false;
            return out;
        }

        bool EventSystem::_redrawRequest(const std::shared_ptr<Widget>& widget) const
        {
            bool out = widget->_redrawRequest;
            widget->_redrawRequest = false;
            return out;
        }

        void EventSystem::_initLayoutRecursive(const std::shared_ptr<Widget>& widget, Event::InitLayout& event)
        {
            for (const auto& child : widget->getChildWidgets())
            {
                _initLayoutRecursive(child, event);
            }
            widget->event(event);
        }

        void EventSystem::_preLayoutRecursive(const std::shared_ptr<Widget>& widget, Event::PreLayout& event)
        {
            for (const auto& child : widget->getChildWidgets())
            {
                _preLayoutRecursive(child, event);
            }
            widget->event(event);
        }

        void EventSystem::_layoutRecursive(const std::shared_ptr<Widget>& widget, Event::Layout& event)
        {
            if (widget->isVisible())
            {
                widget->event(event);
                for (const auto& child : widget->getChildWidgets())
                {
                    _layoutRecursive(child, event);
                }
            }
        }

        void EventSystem::_clipRecursive(const std::shared_ptr<Widget>& widget, Event::Clip& event)
        {
            widget->event(event);
            const BBox2f clipRect = event.getClipRect();
            for (const auto& child : widget->getChildWidgets())
            {
                event.setClipRect(clipRect.intersect(child->getGeometry()));
                _clipRecursive(child, event);
            }
            event.setClipRect(clipRect);
        }

        void EventSystem::_paintRecursive(
            const std::shared_ptr<Widget>& widget,
            Event::Paint& event,
            Event::PaintOverlay& overlayEvent)
        {
            if (widget->isVisible() && !widget->isClipped())
            {
                const BBox2f clipRect = event.getClipRect();
                _pushClipRect(clipRect);
                widget->event(event);
                for (const auto& child : widget->getChildWidgets())
                {
                    const BBox2f childClipRect = clipRect.intersect(child->getGeometry());
                    event.setClipRect(childClipRect);
                    overlayEvent.setClipRect(childClipRect);
                    _paintRecursive(child, event, overlayEvent);
                }
                widget->event(overlayEvent);
                _popClipRect();
                event.setClipRect(clipRect);
                overlayEvent.setClipRect(clipRect);
            }
        }

        void EventSystem::_initObject(const std::shared_ptr<IObject>& object)
        {
            IEventSystem::_initObject(object);
            DJV_PRIVATE_PTR();
            if (auto window = std::dynamic_pointer_cast<Window>(object))
            {
                setTextFocus(nullptr);
                getRootObject()->addChild(window);
                p.windows.push_back(window);
            }
        }

    } // namespace UI
} // namespace djv

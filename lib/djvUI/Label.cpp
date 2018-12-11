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

#include <djvUI/Label.h>

#include <djvCore/Memory.h>

#include <djvAV/Render2DSystem.h>

//#pragma optimize("", off)

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        struct Label::Private
        {
            std::string text;
            TextHAlign textHAlign = TextHAlign::Center;
            TextVAlign textVAlign = TextVAlign::Center;
            ColorRole textColorRole = ColorRole::Foreground;
            FontFace fontFace = FontFace::First;
            MetricsRole fontSizeRole = MetricsRole::FontMedium;
            float minimumWidth = 0.f;
            std::future<AV::FontMetrics> fontMetricsFuture;
            glm::vec2 textSize = glm::vec2(0.f, 0.f);
            std::future<glm::vec2> textSizeFuture;
        };

        void Label::_init(const std::shared_ptr<Context>& context)
        {
            Widget::_init(context);
            
            setName("djv::UI::Label");
        }
        
        Label::Label() :
            _p(new Private)
        {}

        Label::~Label()
        {}

        std::shared_ptr<Label> Label::create(const std::shared_ptr<Context>& context)
        {
            auto out = std::shared_ptr<Label>(new Label);
            out->_init(context);
            return out;
        }

        std::shared_ptr<Label> Label::create(const std::string& text, const std::shared_ptr<Context>& context)
        {
            auto out = Label::create(context);
            out->setText(text);
            return out;
        }

        const std::string& Label::getText() const
        {
            return _p->text;
        }

        void Label::setText(const std::string& value)
        {
            _p->text = value;
        }

        TextHAlign Label::getTextHAlign() const
        {
            return _p->textHAlign;
        }
        
        TextVAlign Label::getTextVAlign() const
        {
            return _p->textVAlign;
        }
        
        void Label::setTextHAlign(TextHAlign value)
        {
            _p->textHAlign = value;
        }
        
        void Label::setTextVAlign(TextVAlign value)
        {
            _p->textVAlign = value;
        }
            
        ColorRole Label::getTextColorRole() const
        {
            return _p->textColorRole;
        }

        void Label::setTextColorRole(ColorRole value)
        {
            _p->textColorRole = value;
        }

        FontFace Label::getFontFace() const
        {
            return _p->fontFace;
        }

        MetricsRole Label::getFontSizeRole() const
        {
            return _p->fontSizeRole;
        }

        void Label::setFontFace(FontFace value)
        {
            _p->fontFace = value;
        }

        void Label::setFontSizeRole(MetricsRole value)
        {
            _p->fontSizeRole = value;
        }

        float Label::getMinimumWidth() const
        {
            return _p->minimumWidth;
        }

        void Label::setMinimumWidth(float value)
        {
            _p->minimumWidth = value;
        }

        void Label::_updateEvent(UpdateEvent& event)
        {
            const auto style = _getStyle();
            const AV::Font font = style->getFont(_p->fontFace, _p->fontSizeRole);
            const auto fontSystem = _getFontSystem();
            _p->fontMetricsFuture = fontSystem->getMetrics(font);
            _p->textSizeFuture = fontSystem->measure(_p->text, font);
        }

        void Label::_preLayoutEvent(PreLayoutEvent& event)
        {
            const auto style = _getStyle();
            _p->textSize = glm::ivec2(0, 0);
            if (_p->textSizeFuture.valid())
            {
                try
                {
                    _p->textSize = _p->textSizeFuture.get();
                }
                catch (const std::exception& e)
                {
                    _log(e.what(), LogLevel::Error);
                }
            }
            glm::vec2 size = _p->textSize;
            size.x = std::max(size.x, _p->minimumWidth);
            _setMinimumSize(size + getMargin().getSize(style));
        }

        void Label::_paintEvent(PaintEvent& event)
        {
            Widget::_paintEvent(event);

            auto renderSystem = _getRenderSystem();
            const auto style = _getStyle();
            const BBox2f& g = getMargin().bbox(getGeometry(), style);
            const glm::vec2 c = g.getCenter();

            renderSystem->setCurrentFont(style->getFont(_p->fontFace, _p->fontSizeRole));
            glm::vec2 pos = g.min;
            switch (_p->textHAlign)
            {
            case TextHAlign::Center:
                pos.x = c.x - _p->textSize.x / 2.f;
                break;
            case TextHAlign::Left:
                pos.x = g.min.x;
                break;
            case TextHAlign::Right:
                pos.x = g.max.x - _p->textSize.x;
                break;
            default: break;
            }
            float ascender = 0.f;
            if (_p->fontMetricsFuture.valid())
            {
                try
                {
                    ascender = _p->fontMetricsFuture.get().ascender;
                }
                catch (const std::exception& e)
                {
                    _log(e.what(), LogLevel::Error);
                }
            }
            switch (_p->textVAlign)
            {
            case TextVAlign::Center:
                pos.y = c.y - _p->textSize.y / 2.f;
                break;
            case TextVAlign::Top:
                pos.y = g.min.y;
                break;
            case TextVAlign::Bottom:
                pos.y = g.max.y - _p->textSize.y;
                break;
            case TextVAlign::Baseline:
                pos.y = c.y - ascender / 2.f;
                break;
            default: break;
            }

            renderSystem->setFillColor(style->getColor(_p->textColorRole));
            renderSystem->drawText(_p->text, glm::vec2(pos.x, pos.y + ascender));
        }

    } // namespace UI
} // namespace djv

//------------------------------------------------------------------------------
// Copyright (c) 2004-2019 Darby Johnston
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

#include <djvUI/ToggleButton.h>

#include <djvUI/DrawUtil.h>

#include <djvAV/Render2D.h>

#include <djvCore/Animation.h>

//#pragma optimize("", off)

using namespace djv::Core;

namespace djv
{
    namespace UI
    {
        namespace Button
        {
            namespace
            {
                //! \todo [1.0 S] Should this be configurable?
                const size_t animationDuration = 50;
            
            } // namespace

            struct Toggle::Private
            {
                AV::Font::Metrics fontMetrics;
                std::future<AV::Font::Metrics> fontMetricsFuture;
                float animationValue = 0.f;
                std::shared_ptr<Animation::Animation> animation;
            };

            void Toggle::_init(Context * context)
            {
                IButton::_init(context);

                DJV_PRIVATE_PTR();
                setClassName("djv::UI::Button::Toggle");
                setButtonType(ButtonType::Toggle);
                setHAlign(HAlign::Left);
                setVAlign(VAlign::Center);
                p.animation = Animation::Animation::create(context);
            }

            Toggle::Toggle() :
                _p(new Private)
            {}

            Toggle::~Toggle()
            {}

            std::shared_ptr<Toggle> Toggle::create(Context * context)
            {
                auto out = std::shared_ptr<Toggle>(new Toggle);
                out->_init(context);
                return out;
            }

            void Toggle::setChecked(bool value)
            {
                IButton::setChecked(value);
                DJV_PRIVATE_PTR();
                if (isVisible(true))
                {
                    auto weak = std::weak_ptr<Toggle>(std::dynamic_pointer_cast<Toggle>(shared_from_this()));
                    p.animation->start(
                        p.animationValue,
                        value ? 1.f : 0.f,
                        std::chrono::milliseconds(animationDuration),
                        [weak](float value)
                    {
                        if (auto widget = weak.lock())
                        {
                            widget->_p->animationValue = value;
                            widget->_redraw();
                        }
                    });
                }
            }

            void Toggle::_styleEvent(Event::Style& event)
            {
                DJV_PRIVATE_PTR();
                const auto& style = _getStyle();
                auto fontSystem = _getFontSystem();
                p.fontMetricsFuture = fontSystem->getMetrics(style->getFontInfo(AV::Font::faceDefault, MetricsRole::FontMedium));
            }

            void Toggle::_preLayoutEvent(Event::PreLayout & event)
            {
                DJV_PRIVATE_PTR();
                if (p.fontMetricsFuture.valid())
                {
                    try
                    {
                        p.fontMetrics = p.fontMetricsFuture.get();
                    }
                    catch (const std::exception& e)
                    {
                        _log(e.what(), LogLevel::Error);
                    }
                }
                const auto& style = _getStyle();
                const float m = style->getMetric(Style::MetricsRole::MarginSmall);
                glm::vec2 minimumSize = glm::vec2(0.f, 0.f);
                minimumSize.x = p.fontMetrics.lineHeight * 2.f + m * 2.f;
                minimumSize.y = p.fontMetrics.lineHeight + m * 2.f;
                _setMinimumSize(minimumSize);
            }

            void Toggle::_paintEvent(Event::Paint & event)
            {
                Widget::_paintEvent(event);

                DJV_PRIVATE_PTR();
                const auto& style = _getStyle();
                const float b = style->getMetric(Style::MetricsRole::Border);
                const float m = style->getMetric(Style::MetricsRole::MarginSmall);
                const BBox2f & g = getGeometry();

                auto render = _getRender();
                render->setFillColor(style->getColor(ColorRole::Border));
                const BBox2f g1 = g.margin(-m);
                drawBorder(render, g1, b);

                if (isChecked())
                {
                    render->setFillColor(style->getColor(ColorRole::Checked));
                    render->drawRect(g1.margin(-b));
                }

                const float r = g1.h() / 2.f;
                const float x = Math::lerp(p.animationValue, g1.min.x + r, g1.max.x - r);
                const BBox2f handleBBox(x - r, g1.min.y, r * 2.f, r * 2.f);
                auto color = style->getColor(ColorRole::Border);
                render->setFillColor(color);
                render->drawRect(handleBBox);
                color = style->getColor(ColorRole::Button);
                render->setFillColor(color);
                render->drawRect(handleBBox.margin(-b));
                if (_isPressed())
                {
                    render->setFillColor(style->getColor(ColorRole::Pressed));
                    render->drawRect(handleBBox);
                }
                else if (_isHovered())
                {
                    render->setFillColor(style->getColor(ColorRole::Hovered));
                    render->drawRect(handleBBox);
                }
            }

        } // namespace Button
    } // namespace UI
} // namespace Gp

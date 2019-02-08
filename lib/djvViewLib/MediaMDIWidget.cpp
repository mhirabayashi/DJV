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

#include <djvViewLib/MediaMDIWidget.h>

#include <djvViewLib/ImageView.h>
#include <djvViewLib/MediaWidget.h>
#include <djvViewLib/PlaybackWidget.h>
#include <djvViewLib/TimelineSlider.h>

#include <djvUI/Border.h>
#include <djvUI/Label.h>
#include <djvUI/RowLayout.h>
#include <djvUI/StackLayout.h>
#include <djvUI/ToolButton.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewLib
    {
        struct MediaMDIWidget::Private
        {
            std::shared_ptr<MediaWidget> mediaWidget;
            std::shared_ptr<UI::Label> titleLabel;
            std::shared_ptr<UI::HorizontalLayout> titleBar;
            std::shared_ptr<UI::StackLayout> layout;
            std::shared_ptr<UI::Border> border;
            std::function<void(void)> maximizeCallback;
            std::function<void(void)> closeCallback;
        };
        
        void MediaMDIWidget::_init(Context * context)
        {
            IWidget::_init(context);

            DJV_PRIVATE_PTR();
            p.mediaWidget = MediaWidget::create(context);

            p.titleLabel = UI::Label::create(context);
            p.titleLabel->setTextHAlign(UI::TextHAlign::Left);
            p.titleLabel->setMargin(UI::MetricsRole::Margin);

            auto maximizeButton = UI::ToolButton::create(context);
            maximizeButton->setIcon("djvIconViewLibSDISmall");

            auto closeButton = UI::ToolButton::create(context);
            closeButton->setIcon("djvIconCloseSmall");

            p.titleBar = UI::HorizontalLayout::create(context);
            p.titleBar->setBackgroundRole(UI::ColorRole::Overlay);
            p.titleBar->addWidget(p.titleLabel, UI::RowStretch::Expand);
            auto hLayout = UI::HorizontalLayout::create(context);
            hLayout->setSpacing(UI::MetricsRole::None);
            hLayout->addWidget(maximizeButton);
            hLayout->addWidget(closeButton);
            p.titleBar->addWidget(hLayout);

            p.layout = UI::StackLayout::create(context);
            p.layout->addWidget(p.mediaWidget);
            auto vLayout = UI::VerticalLayout::create(context);
            vLayout->setSpacing(UI::MetricsRole::None);
            vLayout->addWidget(p.titleBar);
            vLayout->addExpander();
            p.layout->addWidget(vLayout);

            p.border = UI::Border::create(context);
            p.border->setMargin(UI::MetricsRole::Handle);
            p.border->addWidget(p.layout);
            IContainer::addWidget(p.border);

            auto weak = std::weak_ptr<MediaMDIWidget>(std::dynamic_pointer_cast<MediaMDIWidget>(shared_from_this()));
            maximizeButton->setClickedCallback(
                [weak]
            {
                if (auto widget = weak.lock())
                {
                    if (widget->_p->maximizeCallback)
                    {
                        widget->_p->maximizeCallback();
                    }
                }
            });
            closeButton->setClickedCallback(
                [weak]
            {
                if (auto widget = weak.lock())
                {
                    if (widget->_p->closeCallback)
                    {
                        widget->_p->closeCallback();
                    }
                }
            });
        }

		MediaMDIWidget::MediaMDIWidget() :
            _p(new Private)
        {}

		MediaMDIWidget::~MediaMDIWidget()
        {}

        std::shared_ptr<MediaMDIWidget> MediaMDIWidget::create(Context * context)
        {
            auto out = std::shared_ptr<MediaMDIWidget>(new MediaMDIWidget);
            out->_init(context);
            return out;
        }

        const std::string & MediaMDIWidget::getTitle() const
        {
            return _p->titleLabel->getText();
        }

        void MediaMDIWidget::setTitle(const std::string & text)
        {
            _p->titleLabel->setText(text);
        }

        const std::shared_ptr<Media> & MediaMDIWidget::getMedia() const
        {
            return _p->mediaWidget->getMedia();
        }

        void MediaMDIWidget::setMedia(const std::shared_ptr<Media> & value)
        {
            _p->mediaWidget->setMedia(value);
        }

        void MediaMDIWidget::setCloseCallback(const std::function<void(void)> & value)
        {
            _p->closeCallback = value;
        }

        void MediaMDIWidget::setMaximizeCallback(const std::function<void(void)> & value)
        {
            _p->maximizeCallback = value;
        }

        float MediaMDIWidget::getHeightForWidth(float value) const
        {
            return _p->border->getHeightForWidth(value);
        }

        void MediaMDIWidget::_preLayoutEvent(Event::PreLayout& event)
        {
            _setMinimumSize(_p->border->getMinimumSize());
        }

        void MediaMDIWidget::_layoutEvent(Event::Layout&)
        {
            _p->border->setGeometry(getGeometry());
        }
        
    } // namespace ViewLib
} // namespace djv


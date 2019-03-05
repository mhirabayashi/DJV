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

#include <djvViewLib/AboutDialog.h>

#include <djvUI/GroupBox.h>
#include <djvUI/MDICanvas.h>
#include <djvUI/RowLayout.h>
#include <djvUI/ScrollWidget.h>
#include <djvUI/TextBlock.h>

#include <djvAV/ThumbnailSystem.h>

#include <djvCore/Context.h>

using namespace djv::Core;

namespace djv
{
    namespace ViewLib
    {
        struct AboutDialog::Private
        {
            std::map<std::string, std::shared_ptr<UI::GroupBox> > groupBoxes;
            std::map<std::string, std::shared_ptr<UI::TextBlock> > textBlocks;
            std::future<std::shared_ptr<AV::Image::Image> > imageFuture;
        };

        void AboutDialog::_init(Context * context)
        {
            IDialog::_init(context);

            DJV_PRIVATE_PTR();
            p.textBlocks["Copyright"] = UI::TextBlock::create(context);
            p.textBlocks["License1"] = UI::TextBlock::create(context);
            p.textBlocks["License2"] = UI::TextBlock::create(context);
            p.textBlocks["License3"] = UI::TextBlock::create(context);
            p.textBlocks["License4"] = UI::TextBlock::create(context);
            p.textBlocks["License5"] = UI::TextBlock::create(context);
            p.textBlocks["ContributorsHeader"] = UI::TextBlock::create(context);
            p.textBlocks["ContributorsHeader"]->setFontSizeRole(UI::MetricsRole::FontLarge);
            p.textBlocks["Contributors1"] = UI::TextBlock::create(context);
            p.textBlocks["Contributors2"] = UI::TextBlock::create(context);
            p.textBlocks["Contributors3"] = UI::TextBlock::create(context);
            p.textBlocks["Contributors4"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdPartyHeader"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdPartyHeader"]->setFontSizeRole(UI::MetricsRole::FontLarge);
            p.textBlocks["ThirdParty1"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty2"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty3"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty4"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty5"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty6"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty7"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty8"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty9"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty10"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty11"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty12"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty13"] = UI::TextBlock::create(context);
            p.textBlocks["ThirdParty14"] = UI::TextBlock::create(context);
            p.textBlocks["TrademarksHeader"] = UI::TextBlock::create(context);
            p.textBlocks["TrademarksHeader"]->setFontSizeRole(UI::MetricsRole::FontLarge);
            p.textBlocks["Trademarks1"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks2"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks3"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks4"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks5"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks6"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks7"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks8"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks9"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks10"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks11"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks12"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks13"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks14"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks15"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks16"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks17"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks18"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks19"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks20"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks21"] = UI::TextBlock::create(context);
            p.textBlocks["Trademarks22"] = UI::TextBlock::create(context);
            p.textBlocks["MadeIn"] = UI::TextBlock::create(context);

            auto textLayout = UI::VerticalLayout::create(context);
            textLayout->setSpacing(UI::MetricsRole::SpacingLarge);
            
            auto vLayout = UI::VerticalLayout::create(context);
            vLayout->setMargin(UI::MetricsRole::MarginSmall);
            vLayout->addChild(p.textBlocks["Copyright"]);
            vLayout->addChild(p.textBlocks["License1"]);
            auto vLayout2 = UI::VerticalLayout::create(context);
            vLayout2->setSpacing(UI::MetricsRole::None);
            vLayout2->addChild(p.textBlocks["License2"]);
            vLayout2->addChild(p.textBlocks["License3"]);
            vLayout2->addChild(p.textBlocks["License4"]);
            vLayout->addChild(vLayout2);
            vLayout->addChild(p.textBlocks["License5"]);
            p.groupBoxes["Copyright"] = UI::GroupBox::create(context);
            p.groupBoxes["Copyright"]->addChild(vLayout);
            textLayout->addChild(p.groupBoxes["Copyright"]);
            
            vLayout = UI::VerticalLayout::create(context);
            vLayout->setMargin(UI::MetricsRole::MarginSmall);
            vLayout->addChild(p.textBlocks["Contributors1"]);
            vLayout2 = UI::VerticalLayout::create(context);
            vLayout2->setSpacing(UI::MetricsRole::None);
            vLayout2->addChild(p.textBlocks["Contributors2"]);
            vLayout2->addChild(p.textBlocks["Contributors3"]);
            vLayout2->addChild(p.textBlocks["Contributors4"]);
            vLayout->addChild(vLayout2);
            p.groupBoxes["Contributors"] = UI::GroupBox::create(context);
            p.groupBoxes["Contributors"]->addChild(vLayout);
            textLayout->addChild(p.groupBoxes["Contributors"]);
            
            vLayout = UI::VerticalLayout::create(context);
            vLayout->setMargin(UI::MetricsRole::MarginSmall);
            vLayout->addChild(p.textBlocks["ThirdParty1"]);
            vLayout2 = UI::VerticalLayout::create(context);
            vLayout2->setSpacing(UI::MetricsRole::None);
            vLayout2->addChild(p.textBlocks["ThirdParty2"]);
            vLayout2->addChild(p.textBlocks["ThirdParty3"]);
            vLayout2->addChild(p.textBlocks["ThirdParty4"]);
            vLayout2->addChild(p.textBlocks["ThirdParty5"]);
            vLayout2->addChild(p.textBlocks["ThirdParty6"]);
            vLayout2->addChild(p.textBlocks["ThirdParty7"]);
            vLayout2->addChild(p.textBlocks["ThirdParty8"]);
            vLayout2->addChild(p.textBlocks["ThirdParty9"]);
            vLayout2->addChild(p.textBlocks["ThirdParty10"]);
            vLayout2->addChild(p.textBlocks["ThirdParty11"]);
            vLayout2->addChild(p.textBlocks["ThirdParty12"]);
            vLayout2->addChild(p.textBlocks["ThirdParty13"]);
            vLayout2->addChild(p.textBlocks["ThirdParty14"]);
            vLayout->addChild(vLayout2);
            p.groupBoxes["ThirdParty"] = UI::GroupBox::create(context);
            p.groupBoxes["ThirdParty"]->addChild(vLayout);
            textLayout->addChild(p.groupBoxes["ThirdParty"]);

            vLayout = UI::VerticalLayout::create(context);
            vLayout->setMargin(UI::MetricsRole::MarginSmall);
            vLayout->addChild(p.textBlocks["Trademarks1"]);
            vLayout2 = UI::VerticalLayout::create(context);
            vLayout2->setSpacing(UI::MetricsRole::None);
            vLayout2->addChild(p.textBlocks["Trademarks2"]);
            vLayout2->addChild(p.textBlocks["Trademarks3"]);
            vLayout2->addChild(p.textBlocks["Trademarks4"]);
            vLayout2->addChild(p.textBlocks["Trademarks5"]);
            vLayout2->addChild(p.textBlocks["Trademarks6"]);
            vLayout2->addChild(p.textBlocks["Trademarks7"]);
            vLayout2->addChild(p.textBlocks["Trademarks8"]);
            vLayout2->addChild(p.textBlocks["Trademarks9"]);
            vLayout2->addChild(p.textBlocks["Trademarks10"]);
            vLayout2->addChild(p.textBlocks["Trademarks11"]);
            vLayout2->addChild(p.textBlocks["Trademarks12"]);
            vLayout2->addChild(p.textBlocks["Trademarks13"]);
            vLayout2->addChild(p.textBlocks["Trademarks14"]);
            vLayout2->addChild(p.textBlocks["Trademarks15"]);
            vLayout2->addChild(p.textBlocks["Trademarks16"]);
            vLayout2->addChild(p.textBlocks["Trademarks17"]);
            vLayout2->addChild(p.textBlocks["Trademarks18"]);
            vLayout2->addChild(p.textBlocks["Trademarks19"]);
            vLayout2->addChild(p.textBlocks["Trademarks20"]);
            vLayout2->addChild(p.textBlocks["Trademarks21"]);
            vLayout->addChild(vLayout2);
            vLayout->addChild(p.textBlocks["Trademarks22"]);
            p.groupBoxes["Trademarks"] = UI::GroupBox::create(context);
            p.groupBoxes["Trademarks"]->addChild(vLayout);
            textLayout->addChild(p.groupBoxes["Trademarks"]);

            vLayout = UI::VerticalLayout::create(context);
            vLayout->setSpacing(UI::MetricsRole::None);
            vLayout->addSeparator();
            vLayout2 = UI::VerticalLayout::create(context);
            vLayout2->setMargin(UI::MetricsRole::MarginSmall);
            vLayout2->addChild(p.textBlocks["MadeIn"]);
            vLayout->addChild(vLayout2);
            textLayout->addChild(vLayout);

            auto scrollWidget = UI::ScrollWidget::create(UI::ScrollType::Vertical, context);
            scrollWidget->setBorder(false);
            scrollWidget->addChild(textLayout);
            addChild(scrollWidget);
            setStretch(scrollWidget, UI::RowStretch::Expand);
        }

        AboutDialog::AboutDialog() :
            _p(new Private)
        {}

        AboutDialog::~AboutDialog()
        {}

        std::shared_ptr<AboutDialog> AboutDialog::create(Context * context)
        {
            auto out = std::shared_ptr<AboutDialog>(new AboutDialog);
            out->_init(context);
            return out;
        }

        void AboutDialog::_localeEvent(Event::Locale & event)
        {
            IDialog::_localeEvent(event);
            DJV_PRIVATE_PTR();
            std::stringstream ss;
            ss << _getText(DJV_TEXT("About Title"));
            ss << " " << DJV_VERSION;
            setTitle(ss.str());
            p.groupBoxes["Copyright"]->setText(_getText(DJV_TEXT("Copyright and License")));
            p.textBlocks["Copyright"]->setText(_getText(DJV_TEXT("Copyright")));
            p.textBlocks["License1"]->setText(_getText(DJV_TEXT("License 1")));
            p.textBlocks["License2"]->setText(_getText(DJV_TEXT("License 2")));
            p.textBlocks["License3"]->setText(_getText(DJV_TEXT("License 3")));
            p.textBlocks["License4"]->setText(_getText(DJV_TEXT("License 4")));
            p.textBlocks["License5"]->setText(_getText(DJV_TEXT("License 5")));
            p.groupBoxes["Contributors"]->setText(_getText(DJV_TEXT("Contributors")));
            p.textBlocks["Contributors1"]->setText(_getText(DJV_TEXT("Contributors Text 1")));
            p.textBlocks["Contributors2"]->setText(_getText(DJV_TEXT("Contributors Text 2")));
            p.textBlocks["Contributors3"]->setText(_getText(DJV_TEXT("Contributors Text 3")));
            p.textBlocks["Contributors4"]->setText(_getText(DJV_TEXT("Contributors Text 4")));
            p.groupBoxes["ThirdParty"]->setText(_getText(DJV_TEXT("Third Party")));
            p.textBlocks["ThirdParty1"]->setText(_getText(DJV_TEXT("Third Party Text 1")));
            p.textBlocks["ThirdParty2"]->setText(_getText(DJV_TEXT("Third Party Text 2")));
            p.textBlocks["ThirdParty3"]->setText(_getText(DJV_TEXT("Third Party Text 3")));
            p.textBlocks["ThirdParty4"]->setText(_getText(DJV_TEXT("Third Party Text 4")));
            p.textBlocks["ThirdParty5"]->setText(_getText(DJV_TEXT("Third Party Text 5")));
            p.textBlocks["ThirdParty6"]->setText(_getText(DJV_TEXT("Third Party Text 6")));
            p.textBlocks["ThirdParty7"]->setText(_getText(DJV_TEXT("Third Party Text 7")));
            p.textBlocks["ThirdParty8"]->setText(_getText(DJV_TEXT("Third Party Text 8")));
            p.textBlocks["ThirdParty9"]->setText(_getText(DJV_TEXT("Third Party Text 9")));
            p.textBlocks["ThirdParty10"]->setText(_getText(DJV_TEXT("Third Party Text 10")));
            p.textBlocks["ThirdParty11"]->setText(_getText(DJV_TEXT("Third Party Text 11")));
            p.textBlocks["ThirdParty12"]->setText(_getText(DJV_TEXT("Third Party Text 12")));
            p.textBlocks["ThirdParty13"]->setText(_getText(DJV_TEXT("Third Party Text 13")));
            p.textBlocks["ThirdParty14"]->setText(_getText(DJV_TEXT("Third Party Text 14")));
            p.groupBoxes["Trademarks"]->setText(_getText(DJV_TEXT("Trademarks")));
            p.textBlocks["Trademarks1"]->setText(_getText(DJV_TEXT("Trademarks Text 1")));
            p.textBlocks["Trademarks2"]->setText(_getText(DJV_TEXT("Trademarks Text 2")));
            p.textBlocks["Trademarks3"]->setText(_getText(DJV_TEXT("Trademarks Text 3")));
            p.textBlocks["Trademarks4"]->setText(_getText(DJV_TEXT("Trademarks Text 4")));
            p.textBlocks["Trademarks5"]->setText(_getText(DJV_TEXT("Trademarks Text 5")));
            p.textBlocks["Trademarks6"]->setText(_getText(DJV_TEXT("Trademarks Text 6")));
            p.textBlocks["Trademarks7"]->setText(_getText(DJV_TEXT("Trademarks Text 7")));
            p.textBlocks["Trademarks8"]->setText(_getText(DJV_TEXT("Trademarks Text 8")));
            p.textBlocks["Trademarks9"]->setText(_getText(DJV_TEXT("Trademarks Text 9")));
            p.textBlocks["Trademarks10"]->setText(_getText(DJV_TEXT("Trademarks Text 10")));
            p.textBlocks["Trademarks11"]->setText(_getText(DJV_TEXT("Trademarks Text 11")));
            p.textBlocks["Trademarks12"]->setText(_getText(DJV_TEXT("Trademarks Text 12")));
            p.textBlocks["Trademarks13"]->setText(_getText(DJV_TEXT("Trademarks Text 13")));
            p.textBlocks["Trademarks14"]->setText(_getText(DJV_TEXT("Trademarks Text 14")));
            p.textBlocks["Trademarks15"]->setText(_getText(DJV_TEXT("Trademarks Text 15")));
            p.textBlocks["Trademarks16"]->setText(_getText(DJV_TEXT("Trademarks Text 16")));
            p.textBlocks["Trademarks17"]->setText(_getText(DJV_TEXT("Trademarks Text 17")));
            p.textBlocks["Trademarks18"]->setText(_getText(DJV_TEXT("Trademarks Text 18")));
            p.textBlocks["Trademarks19"]->setText(_getText(DJV_TEXT("Trademarks Text 19")));
            p.textBlocks["Trademarks20"]->setText(_getText(DJV_TEXT("Trademarks Text 20")));
            p.textBlocks["Trademarks21"]->setText(_getText(DJV_TEXT("Trademarks Text 21")));
            p.textBlocks["Trademarks22"]->setText(_getText(DJV_TEXT("Trademarks Text 22")));
            p.textBlocks["MadeIn"]->setText(_getText(DJV_TEXT("Made In")));
        }

    } // namespace ViewLib
} // namespace djv


// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        //! This class provides a tab bar widget.
        class TabBar : public Widget
        {
            DJV_NON_COPYABLE(TabBar);

        protected:
            void _init(const std::shared_ptr<Core::Context>&);
            TabBar();

        public:
            ~TabBar() override;

            static std::shared_ptr<TabBar> create(const std::shared_ptr<Core::Context>&);

            size_t getTabCount() const;
            void setTabs(const std::vector<std::string>&);
            void clearTabs();

            void setText(int, const std::string&);

            int getCurrentTab() const;
            void setCurrentTab(int);
            void setCurrentTabCallback(const std::function<void(int)>&);

            float getHeightForWidth(float) const override;

        protected:
            void _preLayoutEvent(Core::Event::PreLayout&) override;
            void _layoutEvent(Core::Event::Layout&) override;

        private:
            DJV_PRIVATE();
        };

    } // namespace UI
} // namespace djv


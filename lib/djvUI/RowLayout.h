// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUI/Margin.h>
#include <djvUI/Spacing.h>
#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        namespace Layout
        {
            //! This enumeration provides how widgets use space in a layout.
            enum class RowStretch
            {
                None,
                Expand,

                Count,
                First = None
            };
            DJV_ENUM_HELPERS(RowStretch);

            //! This class provides a layout that arranges it's children in a row.
            class Row : public Widget
            {
                DJV_NON_COPYABLE(Row);

            protected:
                void _init(Orientation, const std::shared_ptr<Core::Context>&);
                Row();

            public:
                ~Row() override;

                static std::shared_ptr<Row> create(Orientation, const std::shared_ptr<Core::Context>&);

                void addSeparator();
                void addSpacer(MetricsRole = MetricsRole::Spacing);
                void addExpander();

                Orientation getOrientation() const;
                void setOrientation(Orientation);

                const Spacing& getSpacing() const;
                void setSpacing(const Spacing&);

                RowStretch getStretch(const std::shared_ptr<Widget>&) const;
                void setStretch(const std::shared_ptr<Widget>&, RowStretch);

                float getHeightForWidth(float) const override;

                void addChild(const std::shared_ptr<IObject>&) override;
                void removeChild(const std::shared_ptr<IObject>&) override;

            protected:
                void _preLayoutEvent(Core::Event::PreLayout&) override;
                void _layoutEvent(Core::Event::Layout&) override;

            private:
                DJV_PRIVATE();
            };

            //! This class provides a layout that arranges it's children in a horizontal row.
            class Horizontal : public Row
            {
                DJV_NON_COPYABLE(Horizontal);

            protected:
                void _init(const std::shared_ptr<Core::Context>&);
                Horizontal();

            public:
                static std::shared_ptr<Horizontal> create(const std::shared_ptr<Core::Context>&);
            };

            //! This class provides a layout that arranges it's children in a vertical row.
            class Vertical : public Row
            {
                DJV_NON_COPYABLE(Vertical);

            protected:
                void _init(const std::shared_ptr<Core::Context>&);
                Vertical();

            public:
                static std::shared_ptr<Vertical> create(const std::shared_ptr<Core::Context>&);
            };

        } // namespace Layout

        typedef Layout::Horizontal HorizontalLayout;
        typedef Layout::Vertical VerticalLayout;
        using Layout::RowStretch;

    } // namespace UI

    DJV_ENUM_SERIALIZE_HELPERS(UI::Layout::RowStretch);
    DJV_ENUM_SERIALIZE_HELPERS(UI::Layout::RowStretch);

} // namespace Gp

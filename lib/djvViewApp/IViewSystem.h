// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvViewApp/ViewApp.h>

#include <djvCore/BBox.h>
#include <djvCore/ISystem.h>

#include <functional>
#include <map>

namespace djv
{
    namespace UI
    {
        class Action;
        class ISettingsWidget;
        class Menu;
        struct ShortcutDataPair;
    
        namespace MDI
        {
            class Canvas;

        } // namespace MDI
    } // namespace UI

    namespace ViewApp
    {
        class MDIWidget;

        //! This struct provides menu data.
        struct MenuData
        {
            std::shared_ptr<UI::Menu> menu;
            std::string sortKey;
        };

        //! This class provides the base functionality for systems.
        class IViewSystem : public Core::ISystem
        {
            DJV_NON_COPYABLE(IViewSystem);

        protected:
            void _init(const std::string & name, const std::shared_ptr<Core::Context>&);
            IViewSystem();

        public:
            ~IViewSystem() override;

            virtual std::map<std::string, std::shared_ptr<UI::Action> > getActions() const;
            virtual MenuData getMenu() const;
            virtual std::vector<std::shared_ptr<UI::ISettingsWidget> > createSettingsWidgets() const;

            void setCanvas(const std::shared_ptr<UI::MDI::Canvas>&);

        protected:
            virtual void _openWidget(const std::string&, const std::shared_ptr<MDIWidget>&);
            virtual void _closeWidget(const std::string&);

            const std::map<std::string, std::shared_ptr<MDIWidget> >& _getWidgets() const;
            const std::map<std::string, Core::BBox2f>& _getWidgetGeom() const;
            void _setWidgetGeom(const std::map<std::string, Core::BBox2f>&);

            UI::ShortcutDataPair _getShortcuts(const std::string&) const;
            void _addShortcut(const std::string&, const UI::ShortcutDataPair&);
            void _addShortcut(const std::string&, int key);
            void _addShortcut(const std::string&, int key, int keyModifiers);

            virtual void _textUpdate();
            virtual void _shortcutsUpdate();

        private:
            DJV_PRIVATE();
        };

    } // namespace ViewApp
} // namespace djv


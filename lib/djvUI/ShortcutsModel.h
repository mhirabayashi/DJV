//------------------------------------------------------------------------------
// Copyright (c) 2004-2018 Darby Johnston
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

#pragma once

#include <djvUI/Shortcut.h>

#include <djvCore/Util.h>

#include <QAbstractItemModel>

#include <memory>

namespace djv
{
    namespace UI
    {
        //! This class provides a model for keyboard shorcuts.
        class ShortcutsModel : public QAbstractItemModel
        {
            Q_OBJECT

            //! This property holds the list of shortcuts.
            Q_PROPERTY(
                QVector<Shortcut> shortcuts
                READ              shortcuts
                WRITE             setShortcuts
                NOTIFY            shortcutsChanged)

        public:
            explicit ShortcutsModel(QObject * parent = nullptr);
            ~ShortcutsModel() override;

            //! Get the list of shortcuts.
            const QVector<Shortcut> & shortcuts() const;

            QModelIndex	index(int row, int column, const QModelIndex & parent = QModelIndex()) const override;
            QModelIndex	parent(const QModelIndex & = QModelIndex()) const override;
            Qt::ItemFlags flags(const QModelIndex &) const override;
            QVariant data(const QModelIndex &, int role = Qt::DisplayRole) const override;
            bool setData(const QModelIndex &, const QVariant &, int role) override;
            QVariant headerData(int section, Qt::Orientation, int role = Qt::DisplayRole) const override;
            int rowCount(const QModelIndex & parent = QModelIndex()) const override;
            int columnCount(const QModelIndex & parent = QModelIndex()) const override;

        public Q_SLOTS:
            //! Set the list of shortcuts.
            void setShortcuts(const QVector<djv::UI::Shortcut> &);

        Q_SIGNALS:
            //! This signal is emitted when the list of shortcuts are changed.
            void shortcutsChanged(const QVector<djv::UI::Shortcut> &);

        private:
            void collisionsUpdate();

            DJV_PRIVATE_COPY(ShortcutsModel);

            struct Private;
            std::unique_ptr<Private> _p;
        };

    } // namespace UI
} // namespace djv

//------------------------------------------------------------------------------
// Copyright (c) 2004-2015 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
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

#include <djvViewLib/ViewToolBar.h>

#include <djvViewLib/Context.h>
#include <djvViewLib/ViewActions.h>

#include <djvUI/FloatEdit.h>
#include <djvUI/FloatObject.h>
#include <djvUI/ToolButton.h>

namespace djv
{
    namespace ViewLib
    {
        struct ViewToolBar::Private
        {
            Private() :
                zoomEdit(0)
            {}

            UI::FloatEdit * zoomEdit;
        };

        ViewToolBar::ViewToolBar(
            AbstractActions * actions,
            Context *         context,
            QWidget *         parent) :
            AbstractToolBar(actions, context, parent),
            _p(new Private)
        {
            UI::ToolButton * button = new UI::ToolButton(context);
            button->setDefaultAction(actions->action(ViewActions::ZOOM_IN));
            addWidget(button);

            button = new UI::ToolButton(context);
            button->setDefaultAction(actions->action(ViewActions::ZOOM_OUT));
            addWidget(button);

            button = new UI::ToolButton(context);
            button->setDefaultAction(actions->action(ViewActions::ZOOM_RESET));
            addWidget(button);

            button = new UI::ToolButton(context);
            button->setDefaultAction(actions->action(ViewActions::FIT));
            addWidget(button);

            _p->zoomEdit = new UI::FloatEdit;
            _p->zoomEdit->setRange(.1f, 1000.f);
            _p->zoomEdit->object()->setInc(.1f, .1f);
            addWidget(_p->zoomEdit);
        }

        ViewToolBar::~ViewToolBar()
        {}

        UI::FloatEdit * ViewToolBar::zoomEdit() const
        {
            return _p->zoomEdit;
        }

    } // namespace ViewLib
} // namespace djv

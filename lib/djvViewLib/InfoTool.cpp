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

#include <djvViewLib/InfoTool.h>

#include <djvViewLib/Context.h>

#include <QAction>
#include <QDockWidget>
#include <QMenu>

namespace djv
{
    namespace ViewLib
    {
        namespace
        {
            class Widget : public QWidget
            {
            public:
                Widget()
                {
                    setBackgroundRole(QPalette::Base);
                    setAutoFillBackground(true);
                }

                QSize sizeHint() const override { return QSize(100, 100); }
            };

        } // namespace

        struct InfoTool::Private
        {
        };

        InfoTool::InfoTool(const std::shared_ptr<Context> & context, QObject * parent) :
            IToolObject("InfoTool", context, parent),
            _p(new Private)
        {}

        InfoTool::~InfoTool()
        {}

        QPointer<QDockWidget> InfoTool::createDockWidget()
        {
            auto out = new QDockWidget("Information");
            out->setWidget(new Widget);
            out->hide();
            return out;
        }

        std::string InfoTool::getDockWidgetSortKey() const
        {
            return "3";
        }

        Qt::DockWidgetArea InfoTool::getDockWidgetArea() const
        {
            return Qt::DockWidgetArea::RightDockWidgetArea;
        }

    } // namespace ViewLib
} // namespace djv

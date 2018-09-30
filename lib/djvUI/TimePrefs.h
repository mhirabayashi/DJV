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

#include <djvUI/Core.h>

#include <djvCore/Time.h>

#include <QObject>

namespace djv
{
    namespace UI
    {
        //! This class provides time preferences.
        class TimePrefs : public QObject
        {
            Q_OBJECT

            //! This property holds the global time units.
            Q_PROPERTY(
                djv::Core::Time::UNITS timeUnits
                READ                   timeUnits
                WRITE                  setTimeUnits
                NOTIFY                 timeUnitsChanged)

            //! This property holds the global speed.
            Q_PROPERTY(
                djv::Core::Speed::FPS speed
                READ                  speed
                WRITE                 setSpeed
                NOTIFY                speedChanged)

        public:
            explicit TimePrefs(QObject * parent = nullptr);

            ~TimePrefs();

            //! Get the global time units.
            Core::Time::UNITS timeUnits() const;

            //! Get the global speed.
            Core::Speed::FPS speed() const;

        public Q_SLOTS:
            //! Set the global time units.
            void setTimeUnits(djv::Core::Time::UNITS);

            //! Set the global speed.
            void setSpeed(djv::Core::Speed::FPS);

        Q_SIGNALS:
            //! This signal is emitted when the global time units are changed.
            void timeUnitsChanged(djv::Core::Time::UNITS);

            //! This signal is emitted when the global speed is changed.
            void speedChanged(djv::Core::Speed::FPS);
        };

    } // namespace UI
} // namespace djv

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

#pragma once

#include <djvCore/Enum.h>
#include <djvCore/PicoJSON.h>

namespace djv
{
    namespace ViewApp
    {
        enum class ImageViewLock
        {
            None,
            Fit,
            Center,

            Count,
            First = None
        };
        DJV_ENUM_HELPERS(ImageViewLock);

        enum class ImageRotate
        {
            _0,
            _90,
            _180,
            _270,

            Count,
            First = _0
        };
        DJV_ENUM_HELPERS(ImageRotate);
        float getImageRotate(ImageRotate);

        enum class ImageAspectRatio
        {
            Default,
            Auto,
            _16_9,
            _1_85,
            _2_35,

            Count,
            First = Default
        };
        DJV_ENUM_HELPERS(ImageAspectRatio);
        float getImageAspectRatio(ImageAspectRatio);

        enum class Playback
        {
            Stop,
            Forward,
            Reverse,

            Count,
            First = Forward
        };
        DJV_ENUM_HELPERS(Playback);

        enum class PlaybackMode
        {
            Once,
            Loop,
            PingPong,

            Count,
            First = Once
        };
        DJV_ENUM_HELPERS(PlaybackMode);

    } // namespace ViewApp

    picojson::value toJSON(ViewApp::ImageViewLock);
    picojson::value toJSON(ViewApp::ImageRotate);
    picojson::value toJSON(ViewApp::ImageAspectRatio);

    //! Throws:
    //! - std::exception
    void fromJSON(const picojson::value&, ViewApp::ImageViewLock&);

    //! Throws:
    //! - std::exception
    void fromJSON(const picojson::value&, ViewApp::ImageRotate&);

    //! Throws:
    //! - std::exception
    void fromJSON(const picojson::value&, ViewApp::ImageAspectRatio&);

    DJV_ENUM_SERIALIZE_HELPERS(ViewApp::ImageViewLock);
    DJV_ENUM_SERIALIZE_HELPERS(ViewApp::ImageRotate);
    DJV_ENUM_SERIALIZE_HELPERS(ViewApp::ImageAspectRatio);
    DJV_ENUM_SERIALIZE_HELPERS(ViewApp::Playback);
    DJV_ENUM_SERIALIZE_HELPERS(ViewApp::PlaybackMode);

} // namespace djv

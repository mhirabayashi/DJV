// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2019-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvAV/AV.h>

#include <glm/mat4x4.hpp>

#include <memory>

namespace djv
{
    namespace AV
    {
        namespace Render3D
        {
            //! This class provides the base functionality for cameras.
            class ICamera : public std::enable_shared_from_this<ICamera>
            {
                DJV_NON_COPYABLE(ICamera);

            protected:
                ICamera();

            public:
                virtual ~ICamera() = 0;

                const glm::mat4x4& getV() const;
                const glm::mat4x4& getP() const;

            protected:
                glm::mat4x4 _v = glm::mat4x4(1.F);
                glm::mat4x4 _p = glm::mat4x4(1.F);
            };

            //! This struct provides a default camera.
            class DefaultCamera : public ICamera
            {
            protected:
                DefaultCamera();

            public:
                static std::shared_ptr<DefaultCamera> create();

                void setV(const glm::mat4x4&);
                void setP(const glm::mat4x4&);
            };

        } // namespace Render3D
    } // namespace AV
} // namespace djv

#include <djvAV/Render3DCameraInline.h>

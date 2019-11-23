//------------------------------------------------------------------------------
// Copyright (c) 2019 Darby Johnston
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

#include <djvScene/Scene.h>

#include <djvScene/Camera.h>
#include <djvScene/Primitive.h>

#include <djvAV/Render3D.h>

using namespace djv::Core;

namespace djv
{
    namespace Scene
    {
        void Scene::_init(const std::shared_ptr<Context>& context)
        {

        }

        Scene::Scene()
        {}

        std::shared_ptr<Scene> Scene::create(const std::shared_ptr<Context>& context)
        {
            auto out = std::shared_ptr<Scene>(new Scene);
            out->_init(context);
            return out;
        }

        void Scene::render(
            const std::shared_ptr<AV::Render3D::Render>& render,
            const RenderOptions& renderOptions)
        {
            AV::Render3D::RenderOptions render3DOptions;
            render3DOptions.camera.v = renderOptions.camera->getV();
            render3DOptions.camera.p = renderOptions.camera->getP();
            render3DOptions.size = renderOptions.size;
            render->beginFrame(render3DOptions);
            for (const auto& i : _primitives)
            {
                i->render(render);
            }
            render->endFrame();
        }

    } // namespace Scene
} // namespace djv
#pragma once

#include <memory>
#include "EngineConfig.h"

#if RENDER_API == OGL_API
#include "../Adapter/OGL.h"
#endif

#include "../Adapter/Window.h"
#include "../Adapter/Render.h"


namespace Bored {
    namespace Render {

        /* *
         * Abstract factory to create Render adapters
         * Also a singleton
         * */
        class Manager {
            private:
                Manager();
            public:
                ~Manager() = default;

                static Manager& GetInstance();
                void UseRenderContext(Bored::Window::Window* w);

                std::shared_ptr<VertexArray> CreateVAO();
                std::shared_ptr<VertexBuffer> CreateVBO();
                std::shared_ptr<IndexBuffer> CreateIBO();

                std::shared_ptr<ColorBuffer> CreateCBO();
                std::shared_ptr<RenderBuffer> CreateRBO();
                std::shared_ptr<DepthBuffer> CreateDBO();
                std::shared_ptr<StencilBuffer> CreateSBO();
                std::shared_ptr<FrameBuffer> CreateFBO();

                std::shared_ptr<Texture2D> CreateTexture2D();
                std::shared_ptr<ShaderPipeline> CreateShaderProgram();
            private:
                static std::unique_ptr<Manager> instance_;
                bool isInit = false;
                Context* context;
        };
    }
}

#pragma once

namespace Anwill {

    enum GraphicsAPI
    {
        OpenGL = 1
    };

    class Renderer
    {
    public:
        static void Init(GraphicsAPI api);

        static GraphicsAPI GetAPI();
    private:
        static GraphicsAPI s_API; // This should be an enum or something
    };
}
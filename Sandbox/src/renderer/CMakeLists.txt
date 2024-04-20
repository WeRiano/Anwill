add_library(RendererSandbox STATIC
        BatchRendererHelloWorld.cpp FontHelloWorld.cpp RendererHelloWorld.cpp SpriteAnimationTest.cpp
        TextureHelloWorld.cpp
)

target_link_libraries(RendererSandbox PUBLIC Anwill)
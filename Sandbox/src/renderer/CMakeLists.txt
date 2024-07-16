add_library(RendererSandbox STATIC
        BatchRendererHelloWorld.cpp FontHelloWorld.cpp PrimitivesTest.cpp SpriteAnimationTest.cpp
        TextureHelloWorld.cpp
)

target_link_libraries(RendererSandbox PUBLIC Anwill)
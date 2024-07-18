add_library(RendererSandbox STATIC
        BatchRendererTest.cpp FontTest.cpp PrimitivesTest.cpp SpriteAnimationTest.cpp
        TextureTest.cpp
)

target_link_libraries(RendererSandbox PUBLIC Anwill)
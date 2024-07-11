cmake_minimum_required(VERSION 3.19)
project(AnwillSandbox)

include_directories(src)

add_subdirectory(src/base)
add_subdirectory(src/ecs)
add_subdirectory(src/gui)
add_subdirectory(src/physics)
add_subdirectory(src/renderer)
add_subdirectory(src/tps)

add_library(Sandbox INTERFACE)
target_include_directories(Sandbox INTERFACE src)
target_link_libraries(Sandbox INTERFACE
        Anwill
        SandboxBase
        EcsSandbox
        GuiSandbox
        CollisionSandbox DynamicsSandbox
        RendererSandbox
        TopDownShadow
)
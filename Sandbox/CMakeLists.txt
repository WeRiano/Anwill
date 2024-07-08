project(AnwillSandbox)

include_directories(src)

add_subdirectory(src/base)
add_subdirectory(src/ecs)
add_subdirectory(src/physics)
add_subdirectory(src/renderer)
add_subdirectory(src/tps)

add_executable(Sandbox src/Sandbox.cpp)

target_link_libraries(Sandbox PUBLIC
        Anwill
        SandboxBase
        EcsSandbox
        CollisionSandbox DynamicsSandbox
        RendererSandbox
        TopDownShadow
)
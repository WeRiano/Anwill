add_library(SandboxBase STATIC
        MovingCameraBaseLayer.cpp
        Sandbox.cpp
        SandboxEventHandler.cpp
        SandboxLayer.cpp
)

target_link_libraries(SandboxBase PUBLIC Anwill)
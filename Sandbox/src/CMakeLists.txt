add_executable(Sandbox Sandbox.cpp

        Tests/Ecs/EcsTestLayer.cpp

        Tests/Gui/GuiTest.cpp

        Tests/Physics/dynamics/DynamicsRender.cpp
        Tests/Physics/dynamics/DynamicsTest.cpp
        Tests/Physics/collision/CollisionRender.cpp
        Tests/Physics/collision/CollisionTest.cpp

        Tests/Renderer/RendererHelloWorld.cpp
        Tests/Renderer/TextureHelloWorld.cpp
        Tests/Renderer/FontHelloWorld.cpp
        Tests/Renderer/BatchRendererHelloWorld.cpp

        SmallerProjects/TopDownShadow/TPS.cpp
        SmallerProjects/TopDownShadow/TPSCalcs.cpp
        )

target_include_directories(Sandbox PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/src
        )

target_link_libraries(Sandbox PRIVATE Anwill)

#set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox")
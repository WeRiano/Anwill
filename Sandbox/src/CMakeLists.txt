add_executable(Sandbox Sandbox.cpp

        Tests/Renderer/RendererHelloWorld.cpp Tests/EcsTest/EcsTestLayer.cpp
        Tests/PhysicsTest/dynamics/DynamicsDelta.cpp Tests/PhysicsTest/dynamics/DynamicsExist.cpp
        Tests/PhysicsTest/collision/CollisionPoints.cpp

        SmallerProjects/TopDownShadow/TPS.cpp SmallerProjects/TopDownShadow/TPSCalcs.cpp)

target_include_directories(Sandbox PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/src
        )

target_link_libraries(Sandbox PRIVATE Anwill)

#set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox")
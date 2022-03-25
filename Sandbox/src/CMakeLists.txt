add_executable(Sandbox Sandbox.cpp Layer2D.cpp EcsTestLayer.cpp)

target_include_directories(Sandbox PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/src
        )

target_link_libraries(Sandbox PRIVATE Anwill)

#set_property(TARGET Sandbox PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Sandbox")
add_executable(Sandbox Sandbox.cpp)
target_include_directories(Sandbox PUBLIC ${PROJECT_SOURCE_DIR}/Anwill/src)
target_link_libraries(Sandbox PUBLIC Anwill)
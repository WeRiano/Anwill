add_library(Platform WinWindow.cpp)
target_include_directories(Platform PRIVATE ${PROJECT_SOURCE_DIR}/Anwill/src)
target_link_libraries(Platform PRIVATE Core)
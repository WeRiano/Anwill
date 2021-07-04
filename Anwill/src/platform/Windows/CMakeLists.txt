add_library(Windows WinWindow.cpp)
target_include_directories(Windows PRIVATE ${PROJECT_SOURCE_DIR}/Anwill/src)
target_link_libraries(Windows PRIVATE Core Events)
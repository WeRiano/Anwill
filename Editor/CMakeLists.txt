cmake_minimum_required(VERSION 3.19)
project(AnwillEditor)

include_directories(src)

add_executable(Editor src/Editor.cpp src/EditorLayer.cpp)

target_link_libraries(Editor PUBLIC Anwill)
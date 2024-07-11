cmake_minimum_required(VERSION 3.19)
project(AnwillEditor)

add_executable(Editor src/Editor.cpp src/EditorLayer.cpp src/EditorEventHandler.cpp)
target_link_libraries(Editor PUBLIC Anwill Sandbox)
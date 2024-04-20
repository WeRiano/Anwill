cmake_minimum_required(VERSION 3.19)
project(AnwillEngine)

set(CMAKE_CXX_STANDARD 20)

# Anwill source
add_subdirectory(Anwill)

# Playground
add_subdirectory(Sandbox)

# TODO: Editor!
#add_subdirectory(Editor_project)
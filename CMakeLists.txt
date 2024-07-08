cmake_minimum_required(VERSION 3.19)
project(AnwillEngine)

set(CMAKE_CXX_STANDARD 20)

# Anwill source
add_subdirectory(Anwill)

# TODO: Editor!
#add_subdirectory(Editor_project)

# Playground
add_subdirectory(Sandbox)
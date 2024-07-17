cmake_minimum_required(VERSION 3.19)
project(AnwillEngine)

set(CMAKE_CXX_STANDARD 20)

# Anwill source
add_subdirectory(Anwill)

# Playground // TODO: Remove now that editor exists?
add_subdirectory(Sandbox)

# Editor
add_subdirectory(Editor)

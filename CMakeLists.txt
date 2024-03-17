cmake_minimum_required(VERSION 3.19)
project(AnwillEngine)

set(CMAKE_CXX_STANDARD 20)

# Anwill source
add_subdirectory(Anwill/src)

# Test environment
add_subdirectory(Sandbox/src)

# Dependencies
#add_subdirectory(Anwill/vendor/glfw)
cmake_minimum_required(VERSION 3.19)
project(Anwill)

# All Anwill files should be able to work their includes from this directory
include_directories(src)

# library subsystems
add_subdirectory(src/core)
add_subdirectory(src/ecs)
add_subdirectory(src/events)
add_subdirectory(src/gfx)
add_subdirectory(src/math)
add_subdirectory(src/physics)
add_subdirectory(src/platform)
add_subdirectory(src/utils)

# dependencies
add_subdirectory(vendor/glfw)

add_library(Anwill INTERFACE)
target_link_libraries(Anwill INTERFACE Core Ecs Events Graphics Math Physics Platform Utils)
target_include_directories(Anwill INTERFACE src)

# TODO: "Optimize" PUBLIC usage?? Is it really necessary everywhere? Does it even make a difference if it is everywhere?
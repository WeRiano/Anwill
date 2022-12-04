# All Anwill files should be able to work their includes from this directory
include_directories(${PROJECT_SOURCE_DIR}/Anwill/src)

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(events)
add_subdirectory(gfx)
add_subdirectory(gui)
add_subdirectory(math)
add_subdirectory(physics)
add_subdirectory(platform)
add_subdirectory(utils)

add_library(Anwill INTERFACE)
target_link_libraries(Anwill INTERFACE Core Ecs Events Graphics Gui Math Physics Platform Utils)

# TODO: "Optimize" PUBLIC usage?? Is it really necessary everywhere? Does it even make a difference if it is everywhere?
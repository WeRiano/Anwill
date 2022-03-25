# All Anwill files should be able to work their includes from this directory
include_directories(${PROJECT_SOURCE_DIR}/Anwill/src)

add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(events)
add_subdirectory(platform)
add_subdirectory(math)
add_subdirectory(gfx)
add_subdirectory(utils)

add_library(Anwill INTERFACE)
target_link_libraries(Anwill INTERFACE Core Ecs Events Math Platform Graphics Utils)

# TODO: "Optimize" PUBLIC usage?? Is it really necessary everywhere?
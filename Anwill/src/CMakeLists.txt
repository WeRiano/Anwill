add_subdirectory(core)
add_subdirectory(platform)

add_library(Anwill INTERFACE)
target_link_libraries(Anwill INTERFACE Core)
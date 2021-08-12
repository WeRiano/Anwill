add_subdirectory(core)
add_subdirectory(events)
add_subdirectory(platform)
add_subdirectory(gfx)
add_subdirectory(utils)

add_library(Anwill INTERFACE)
target_link_libraries(Anwill INTERFACE Core Graphics Events Utils)
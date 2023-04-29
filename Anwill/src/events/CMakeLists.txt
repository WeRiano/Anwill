add_library(Events STATIC
        Event.cpp
        SystemEvents.cpp
        )

target_link_libraries(Events PUBLIC Core Utils)

# TODO: Probably should use the logging system :D

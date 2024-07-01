add_library(Events STATIC
        Event.cpp
        SystemEventHandler.cpp
        GuiEventHandler.cpp
        )

target_link_libraries(Events PUBLIC Core Utils)

# TODO: Probably should use the logging system :D

add_library(Utils STATIC
        FileIO.cpp
        Random.cpp
        Utils.cpp
        Profiler.cpp
        )

target_link_libraries(Utils PUBLIC Core)
add_library(Utils STATIC
        FileReader.cpp
        Random.cpp
        )

target_link_libraries(Utils PUBLIC Core)
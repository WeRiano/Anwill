add_library(Utils STATIC
        FileReader.cpp
        Random.cpp
        Utils.cpp
        )

target_link_libraries(Utils PUBLIC Core)
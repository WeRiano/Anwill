add_library(Utils STATIC
        FileReader.cpp
        )

target_link_libraries(Utils PUBLIC Core)
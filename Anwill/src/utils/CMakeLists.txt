add_library(Utils STATIC
        FileReader.cpp)
target_include_directories(Utils PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/src
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Willog/include)
target_link_libraries(Utils PRIVATE)
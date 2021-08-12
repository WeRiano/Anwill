add_library(Core STATIC Log.cpp App.cpp Window.cpp LayerStack.cpp)
target_include_directories(Core PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Willog/include
        ${PROJECT_SOURCE_DIR}/Anwill/src
        )
target_link_libraries(Core PUBLIC Platform Events)
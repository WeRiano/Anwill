add_library(Core STATIC
        App.cpp Window.cpp LayerStack.cpp Log.cpp
        )

target_include_directories(Core PUBLIC
        ${PROJECT_SOURCE_DIR}/Anwill/vendor/Willog/include
        )

target_link_libraries(Core PUBLIC Anwill)
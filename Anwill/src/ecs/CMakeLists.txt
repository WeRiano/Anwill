add_library(Ecs STATIC
        Ecs.cpp EntityManager.cpp
        )
target_link_libraries(Ecs PUBLIC Core)
add_library(Ecs
        Ecs.cpp EntityManager.cpp Entity.cpp Component.cpp
        )

target_link_libraries(Ecs Core)
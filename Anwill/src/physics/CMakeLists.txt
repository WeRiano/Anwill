add_library(Physics STATIC
        Dynamics.cpp
        collision/CircleCollider.cpp collision/PolygonCollider.cpp
        collision/Collision.cpp)

target_link_libraries(Physics PUBLIC Math)
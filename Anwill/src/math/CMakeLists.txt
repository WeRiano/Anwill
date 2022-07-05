add_library(Math STATIC
        Mat4f.cpp Vec3f.cpp Vec2f.cpp Math.cpp #PLine2D.cpp
        )

target_link_libraries(Math PUBLIC
        )
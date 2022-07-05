add_library(Physics STATIC Dynamics.cpp)

target_link_libraries(Physics PUBLIC Math)
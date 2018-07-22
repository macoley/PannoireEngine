
set(TARGET pe)

add_executable(${TARGET} main.cpp Engine.h Engine.cpp)

add_subdirectory(ECS)
add_subdirectory(Render)
add_subdirectory(Resource)
add_subdirectory(Component)

target_link_libraries(${TARGET} PRIVATE pe_ecs pe_render pe_resource)
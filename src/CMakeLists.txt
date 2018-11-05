
set(TARGET pe)

add_executable(${TARGET} main.cpp ../include/PE/ECS/Entity.h)

add_subdirectory(Engine)
add_subdirectory(ECS)
add_subdirectory(Render)
add_subdirectory(Resource)
add_subdirectory(Utils)

target_link_libraries(${TARGET} PRIVATE pe_engine pe_ecs pe_render pe_resource pe_utils)
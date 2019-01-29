
set(TARGET pe)

add_executable(${TARGET} main.cpp)

add_subdirectory(Engine)
add_subdirectory(ECS)
add_subdirectory(Render)
add_subdirectory(Resource)
add_subdirectory(Utils)
add_subdirectory(Scripting)

target_link_libraries(${TARGET} PRIVATE pe_engine pe_ecs pe_render pe_resource pe_utils pe_scripting)
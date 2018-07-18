
set(TARGET pe)

add_executable(${TARGET} main.cpp)

add_subdirectory(Engine)
add_subdirectory(Render)

target_link_libraries(${TARGET} PRIVATE pe_engine pe_render)

set(TARGET pe_engine)

add_executable(${TARGET} main.cpp)

add_subdirectory(Render)

target_link_libraries(${TARGET} PRIVATE pe_render)
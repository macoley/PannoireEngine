
set(TARGET sandbox)

add_executable(${TARGET} main.cpp)

target_link_libraries(${TARGET} PRIVATE duktape dukglue)
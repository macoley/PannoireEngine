cmake_minimum_required(VERSION 3.10)
project(PannoireEngine)

set(CMAKE_CXX_STANDARD 17)

# Conan Package Manager
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

add_subdirectory(src/PE)
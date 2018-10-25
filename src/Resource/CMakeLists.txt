
set(TARGET pe_resource)
set(DIR Resource)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/ResourceManager.h
        ${SRCROOT}/ResourceManager.cpp
        ${INCROOT}/Utils.h
        ${INCROOT}/ResourcePool.h
        ${INCROOT}/ResourceHandle.h
        ${SRCROOT}/ResourceHandle.cpp
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC pe_utils CONAN_PKG::gsl_microsoft CONAN_PKG::stb)


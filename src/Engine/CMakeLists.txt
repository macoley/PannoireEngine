
set(TARGET pe_engine)
set(DIR Engine)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/Core.h
        ${SRCROOT}/Core.cpp
        ${INCROOT}/Scene.h
        ${SRCROOT}/Scene.cpp
        ${INCROOT}/Properties.h
        ${SRCROOT}/Properties.cpp
        ${INCROOT}/LoggerDecorator.h
        ${INCROOT}/Component.h
        ${INCROOT}/Engine.h
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC pe_utils pe_ecs pe_resource pe_render CONAN_PKG::yaml-cpp)



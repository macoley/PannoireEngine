
set(TARGET pe_ecs)
set(DIR ECS)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/ECS.h
        ${SRCROOT}/ECS.cpp
        ${INCROOT}/Manager.h
        ${SRCROOT}/Manager.cpp
        ${INCROOT}/Defines.h
        ${INCROOT}/Component.h
        ${SRCROOT}/Component.cpp
        ${INCROOT}/ComponentSet.h
        ${SRCROOT}/ComponentSet.cpp
        ${INCROOT}/View.h
        ${SRCROOT}/View.cpp
        ${INCROOT}/Entity.h
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC CONAN_PKG::gsl_microsoft pe_utils)


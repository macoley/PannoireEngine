
set(TARGET pe_ecs)
set(DIR ECS)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/Pool.h
        ${SRCROOT}/Pool.cpp
        ${INCROOT}/ECS.h
        ${SRCROOT}/ECS.cpp
        ${INCROOT}/Entity.h
        ${INCROOT}/System.h
        ${INCROOT}/Component.h
        ${SRCROOT}/Component.cpp
        ${INCROOT}/Common.h
        ${INCROOT}/Utils.h
        ${SRCROOT}/System.cpp
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC CONAN_PKG::gsl_microsoft SimpleSignal)



set(TARGET pe_engine)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/Engine)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Engine)

set(SRC
        ${INCROOT}/Engine.h
        ${SRCROOT}/Engine.cpp
        ${INCROOT}/Pool.h
        ${SRCROOT}/Pool.cpp
        ${INCROOT}/ECS.h
        ${SRCROOT}/ECS.cpp
        ${INCROOT}/ComponentType.h
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC CONAN_PKG::gsl_microsoft pe_render SimpleSignal)



set(TARGET pe_utils)
set(DIR Utils)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/Locator.h
        ${SRCROOT}/Locator.cpp
        ${INCROOT}/ILogger.h
        ${INCROOT}/ConsoleLogger.h
        ${SRCROOT}/ConsoleLogger.cpp
        ${INCROOT}/Utils.h
        ${SRCROOT}/Utils.cpp
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC CONAN_PKG::gsl_microsoft)


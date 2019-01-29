
set(TARGET pe_scripting)
set(DIR Scripting)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/Script.h
        ${SRCROOT}/Script.cpp
        ${INCROOT}/ScriptEngine.h
        ${SRCROOT}/ScriptEngine.cpp
        ${INCROOT}/Scripting.h
        ${SRCROOT}/Scripting.cpp
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC duktape dukglue pe_utils)


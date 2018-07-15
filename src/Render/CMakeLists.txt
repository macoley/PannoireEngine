
set(TARGET pe_render)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/Render)
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/Render)

set(SRC
        ${INCROOT}/RenderSystem.h
        ${SRCROOT}/RenderSystem.cpp
        ${INCROOT}/Context.h
        ${SRCROOT}/Context.cpp )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${TARGET} PUBLIC CONAN_PKG::glad CONAN_PKG::glfw CONAN_PKG::glm CONAN_PKG::Assimp CONAN_PKG::gsl_microsoft)


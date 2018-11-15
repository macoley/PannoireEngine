
set(TARGET pe_render)
set(DIR Render)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/Defines.h
        ${INCROOT}/Render.h
        ${SRCROOT}/Render.cpp
        #${INCROOT}/RenderSystem.h
        #${SRCROOT}/RenderSystem.cpp
        ${INCROOT}/Context.h
        ${SRCROOT}/Context.cpp
        ${INCROOT}/Shader.h
        ${SRCROOT}/Shader.cpp
        ${INCROOT}/Mesh.h
        ${SRCROOT}/Mesh.cpp
        ${INCROOT}/Model.h
        ${SRCROOT}/Model.cpp
        ${INCROOT}/Camera.h
        ${SRCROOT}/Camera.cpp
        ${INCROOT}/Texture.h
        ${SRCROOT}/Texture.cpp
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_compile_definitions(${TARGET} PRIVATE STB_IMAGE_IMPLEMENTATION)

target_link_libraries(${TARGET} PUBLIC pe_utils pe_resource CONAN_PKG::glad CONAN_PKG::glfw CONAN_PKG::glm CONAN_PKG::Assimp CONAN_PKG::gsl_microsoft CONAN_PKG::stb)


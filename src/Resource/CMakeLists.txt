
set(TARGET pe_resource)
set(DIR Resource)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/PE/${DIR})
set(SRCROOT ${PROJECT_SOURCE_DIR}/src/${DIR})

set(SRC
        ${INCROOT}/ResourceManager.h
        ${SRCROOT}/ResourceManager.cpp
        ${INCROOT}/ResourceHandle.h
        ${SRCROOT}/ResourceHandle.cpp
        ${INCROOT}/ResourceData.h
        ${INCROOT}/Utils.h
        ${SRCROOT}/Utils.cpp
        ${INCROOT}/Exporter.h
        ${SRCROOT}/Exporter.cpp
        ${INCROOT}/Importer.h
        ${SRCROOT}/Importer.cpp
        ${INCROOT}/Types/BaseResource.h
        ${INCROOT}/Types/TextureResource.h
        ${SRCROOT}/Types/TextureResource.cpp
        ${INCROOT}/Resource.h
        #${SRCROOT}/Loader.cpp
        #${INCROOT}/ResourceData.h
        #${SRCROOT}/ResourceData.cpp
        )

add_library(${TARGET} SHARED ${SRC})

target_include_directories(${TARGET} PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${TARGET} PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_compile_definitions(${TARGET} PRIVATE STB_IMAGE_IMPLEMENTATION)

target_link_libraries(${TARGET} PUBLIC pe_render CONAN_PKG::gsl_microsoft CONAN_PKG::stb)


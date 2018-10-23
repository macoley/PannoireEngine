set(MODULES
        pe_ecs
        pe_render
        pe_resource
        )

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} CONAN_PKG::gtest ${MODULES})
    add_test(${TESTNAME} ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

package_add_test(Test_Resource Test_Resource.cpp)
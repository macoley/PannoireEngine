
function(PE_ADD_TEST)
    cmake_parse_arguments(PE_ADD_TEST "" "TESTNAME" "FILES;MODULES" ${ARGN})

    add_executable(${PE_ADD_TEST_TESTNAME} ${PE_ADD_TEST_FILES})
    target_link_libraries(${PE_ADD_TEST_TESTNAME} CONAN_PKG::gtest ${PE_ADD_TEST_MODULES})
    add_test(${PE_ADD_TEST_TESTNAME} ${PE_ADD_TEST_TESTNAME})
    set_target_properties(${PE_ADD_TEST_TESTNAME} PROPERTIES FOLDER test)
endfunction()

pe_add_test(TESTNAME Test_ECS FILES Test_ECS.cpp MODULES pe_ecs)
#pe_add_test(TESTNAME Test_Resource FILES Test_Resource.cpp MODULES pe_render pe_resource pe_engine)
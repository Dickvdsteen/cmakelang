include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMake_BINARY_DIR}/Source
  ${CMake_SOURCE_DIR}/Source
  ${CMake_SOURCE_DIR}/Source/CLint
  )

set(CLintLib_TESTS
  testXX.cxx)

create_test_sourcelist(CLintLib_TEST_SRCS CLintLibTests.cxx ${CLintLib_TESTS})
add_executable(CLintLibTests ${CLintLib_TEST_SRCS})
target_link_libraries(CLintLibTests CLintLib CTestLib)

set_property(TARGET CLintLibTests PROPERTY C_CLANG_TIDY "")
set_property(TARGET CLintLibTests PROPERTY CXX_CLANG_TIDY "")

foreach(testfile ${CLintLib_TESTS})
  get_filename_component(test "${testfile}" NAME_WE)
  add_test(NAME CLintLib.${test} COMMAND CLintLibTests ${test} ${${test}_ARGS})
endforeach()
#
# Test source files
#
file(GLOB TEST_SOURCES "*.cc" "*.cpp")

# 
# Test executables
#   Assuming only one source file for each executable
# 
foreach(file ${TEST_SOURCES})
  get_filename_component(TEST_EXE ${file} NAME_WE)
  add_executable(${TEST_EXE} ${file})
  target_link_libraries(${TEST_EXE} PUBLIC RexBDD)
  target_include_directories(${TEST_EXE} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
  )
endforeach()

#
# Allow quoted test names with spaces
#
cmake_policy(SET CMP0110 NEW)

#
# Add the tests to run.
# Not automated, in case tests take arguments / input files.
#
add_test("check test"   test_check)


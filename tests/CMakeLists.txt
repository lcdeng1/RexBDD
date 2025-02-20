#
# Test source files
#
file(GLOB TEST_SOURCES "*.cc" "*.cpp")

# Add compile options
if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wshadow -Wunused -Wno-c++11-extensions -Wno-unused-parameter)
endif()

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
  add_test("${TEST_EXE}" ${TEST_EXE})
endforeach()

#
# Allow quoted test names with spaces
#
cmake_policy(SET CMP0110 NEW)

#
# Add the tests to run.
# Not automated, in case tests take arguments / input files.
#
# add_test(NAME your_test_name
#         COMMAND your_test_program your_arguments_or_inputs)


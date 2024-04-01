#
# Example source files
#
file(GLOB EXECUTABLE_SOURCES "*.cc" "*.cpp")

#
# Example executables
#   Assuming only one source file for each executable
#
foreach(file ${EXECUTABLE_SOURCES})
  get_filename_component(EXE ${file} NAME_WE)
  add_executable(${EXE} ${file})
  target_link_libraries(${EXE} PUBLIC RexBDD)
  target_include_directories(${EXE} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
  )
endforeach()

#
# Allow quoted test names with spaces
#
cmake_policy(SET CMP0110 NEW)
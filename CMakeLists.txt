cmake_minimum_required(VERSION 3.19)

project(RexBDD)

# Add compile options
if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wshadow -Wunused)
endif()

enable_testing()

# Add subdirectories in build 
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)




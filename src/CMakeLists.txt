# Add source files
file(GLOB_RECURSE SRC_FILES "*.cc" "*.cpp")
file(GLOB_RECURSE HEADER_FILES "*.h")

# Add compile options
if (MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -Wshadow -Wunused -Wno-c++11-extensions -Wno-unused-parameter)
endif()

# Create a library target
add_library(RexBDD ${SRC_FILES})

# Add include directories
target_include_directories(RexBDD
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)
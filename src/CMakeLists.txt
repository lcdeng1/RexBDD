# Add source files
file(GLOB SRC_FILES "*.cc" "*.cpp")

# Create a library target
add_library(RexBDD ${SRC_FILES})

# Add include directories
target_include_directories(RexBDD
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)
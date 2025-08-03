# WH Core Library - Clean Flat Structure
file(GLOB WH_SOURCES "*.cc")
# Remove inline files that are included by headers to avoid double compilation
list(FILTER WH_SOURCES EXCLUDE REGEX ".*_inline\\.cc$")

add_library(WH ${WH_SOURCES})

# Include parent directory so WH/ prefix works
target_include_directories(WH PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
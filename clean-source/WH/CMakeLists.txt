# WH Library CMakeLists.txt
# Adventure CAD Geometric Modeling Library

# Collect all source files (excluding inline implementations)
file(GLOB WH_SOURCES
    "*.cc"
)

# Remove inline implementation files (they're included by headers)
list(FILTER WH_SOURCES EXCLUDE REGEX ".*_inline\\.cc$")

# Collect all header files
file(GLOB WH_HEADERS
    "*.h"
)

# Create the WH static library
add_library(WH STATIC ${WH_SOURCES} ${WH_HEADERS})

# Set include directories
target_include_directories(WH 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Set target properties
set_target_properties(WH PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    POSITION_INDEPENDENT_CODE ON
)

# Add required preprocessor definitions for WH inline support
target_compile_definitions(WH PUBLIC 
    WH_PRE_ONLY 
    WH_INLINE_ENABLED
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(WH PRIVATE
        -Wall -Wextra -Wpedantic
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(WH PRIVATE
        /W4
    )
endif()

# Debug output control
option(WH_DEBUG_ENABLED "Enable WH debug output" OFF)
if(WH_DEBUG_ENABLED)
    target_compile_definitions(WH PUBLIC WH_DEBUG_ENABLED)
endif()

# Link with math library on Unix-like systems
if(UNIX)
    target_link_libraries(WH PRIVATE m)
endif()

# Install rules
install(TARGETS WH
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${WH_HEADERS}
    DESTINATION include/WH
)
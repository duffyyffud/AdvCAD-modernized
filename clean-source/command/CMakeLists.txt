# Command CMakeLists.txt
# Adventure CAD Command-line Tool

# Define the executable source files
set(ADVCAD_SOURCES
    advcad.cc
)

# Create the advcad executable
add_executable(advcad ${ADVCAD_SOURCES})

# Link with the WH library
target_link_libraries(advcad 
    PRIVATE 
        WH
)

# Set target properties
set_target_properties(advcad PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(advcad PRIVATE
        -Wall -Wextra -Wpedantic
    )
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(advcad PRIVATE
        /W4
    )
endif()

# Install rules
install(TARGETS advcad
    RUNTIME DESTINATION bin
)

# Add custom target for testing with sample files
add_custom_target(test_advcad
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/advcad 
            ${CMAKE_SOURCE_DIR}/sample/block.gm3d 
            ${CMAKE_CURRENT_BINARY_DIR}/test_output.pch 
            2.0
    DEPENDS advcad
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running advcad with sample block geometry"
)

# Add mesh optimizer as auxiliary script
configure_file(
    ${CMAKE_SOURCE_DIR}/optimize_mesh_size.py
    ${CMAKE_CURRENT_BINARY_DIR}/optimize_mesh_size.py
    COPYONLY
)
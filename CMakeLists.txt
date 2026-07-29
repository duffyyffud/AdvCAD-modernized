cmake_minimum_required(VERSION 3.15)
project(AdvCAD VERSION 0.12 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(ENABLE_TESTING "Build tests" OFF)
option(ENABLE_STATIC_ANALYSIS "Enable static analysis" OFF)

# Compiler flags for different build types
## WH_PRE_ONLY is the sole responsibility of WH/CMakeLists.txt's WH_PRE_ONLY_DEFAULT
## option (target_compile_definitions on the WH target); it used to be duplicated
## here too, which silently overrode that option's OFF setting for Debug builds
## (dev/case_records/branch_pipe_afront879.json).
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -Wextra -Wpedantic -DNDEBUG -DWH_INLINE_ENABLED")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -Wall -Wextra -Wpedantic -DNDEBUG -DWH_INLINE_ENABLED")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG /DWH_INLINE_ENABLED")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /DNDEBUG /DWH_INLINE_ENABLED")
endif()

# Add WH library subdirectory
add_subdirectory(WH)

# Add command subdirectory
add_subdirectory(command)

# Testing
if(ENABLE_TESTING)
    enable_testing()
    find_package(GTest)
    if(GTest_FOUND)
        add_subdirectory(tests)
    endif()
endif()
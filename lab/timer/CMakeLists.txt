cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME timer)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    # This dreadful mess is to communicate to clang-tidy the C++ system includes. It seems that CMake
    # doesn't support using its own compile_commands.json database, and that clang-tidy doesn't
    # pick up non-default system headers.
    string(LENGTH ${TOOLCHAIN_PREFIX} PREFIX_LENGTH)
    MATH(EXPR PREFIX_LENGTH "${PREFIX_LENGTH}-1")
    string(SUBSTRING ${TOOLCHAIN_PREFIX} 0 ${PREFIX_LENGTH} TOOLCHAIN_TRIPLE)
    execute_process(
            COMMAND cmd.exe /c ${CMAKE_C_COMPILER} -x c++ -Wp,-v 1 2>&1 | grep "^ .*${TOOLCHAIN_TRIPLE}.*"
            OUTPUT_VARIABLE COMPILER_HEADERS
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(NOTICE "Find system includes: ${COMPILER_HEADERS}")
    string(REGEX REPLACE "[ \n\t]+" ";" INCLUDE_COMPILER_HEADERS ${COMPILER_HEADERS})

    set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${INCLUDE_COMPILER_HEADERS})
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${INCLUDE_COMPILER_HEADERS})
endif()

# Core project settings
project(${CMAKE_PROJECT_NAME})
# Enable CMake support for ASM and C languages
enable_language(C ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

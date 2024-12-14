cmake_minimum_required(VERSION 3.10)
project(globallyRigidPinning)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
file(GLOB SOURCES "*.cpp")

# Add the executable
add_executable(globallyRigidPinning ${SOURCES})

# Set compiler flags for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

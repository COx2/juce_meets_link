cmake_minimum_required(VERSION 3.15)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "osx deployment target")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Set project name used as solution name 
project(juce_meets_link VERSION 0.0.1)

# If you've put JUCE in a subdirectory called JUCE
add_subdirectory(External/JUCE)

# Add Ableton Link libraries to the target using the function from the SDK.
add_subdirectory(External/link)

# Include sub projects
include(./Examples/JuceLink/JuceLink.cmake)

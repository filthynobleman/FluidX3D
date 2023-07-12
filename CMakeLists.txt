cmake_minimum_required(VERSION 3.16.0)
project(FluidX3D LANGUAGES C CXX)

# Avoid mid-level directory to be created under MSVC
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin" CACHE INTERNAL "Where to place executables and dlls")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin" CACHE INTERNAL "Where to place executables and dlls in release mode")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin")

# # MSVC complains about shared libraries
# if(MSVC)
#     set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
#     set(BUILD_SHARED_LIBS TRUE)
# endif()


# Include external dependencies
include_directories("${CMAKE_SOURCE_DIR}/src/OpenCL/include")
include_directories("${CMAKE_SOURCE_DIR}/src/X11/include")

find_package(OpenCL REQUIRED)

# Include project dependencies
include_directories("${CMAKE_COURCE_DIR}/src")


# Source file to library
add_library(FX3D STATIC "${CMAKE_SOURCE_DIR}/src/graphics.cpp"
                        "${CMAKE_SOURCE_DIR}/src/info.cpp"
                        "${CMAKE_SOURCE_DIR}/src/kernel.cpp"
                        "${CMAKE_SOURCE_DIR}/src/lbm.cpp"
                        "${CMAKE_SOURCE_DIR}/src/lodepng.cpp"
                        "${CMAKE_SOURCE_DIR}/src/setup.cpp"
                        "${CMAKE_SOURCE_DIR}/src/shapes.cpp")
target_link_libraries(FX3D OpenCL::OpenCL)
set_property(TARGET FX3D PROPERTY CXX_STANDARD 17) 



# Executable
add_executable(FluidX3D "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_link_libraries(FluidX3D FX3D OpenCL::OpenCL)
set_property(TARGET FluidX3D PROPERTY CXX_STANDARD 17) 

# Copy the skybox to the exe directory
add_custom_command(
        TARGET FluidX3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/skybox/skybox8k.png
                ${CMAKE_SOURCE_DIR}/bin/skybox/skybox8k.png)
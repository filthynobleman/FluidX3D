cmake_minimum_required(VERSION 3.16.0)
project(FluidX3D LANGUAGES C CXX)

# Avoid mid-level directory to be created under MSVC
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin" CACHE INTERNAL "Where to place executables and dlls")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin" CACHE INTERNAL "Where to place executables and dlls in release mode")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_SOURCE_DIR}/bin")

# # MSVC complains about shared libraries
if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)
endif()


# Include external dependencies
include_directories("${PROJECT_SOURCE_DIR}/src/X11/include")
include_directories("${PROJECT_SOURCE_DIR}/ext/json/include")

find_package(OpenCL REQUIRED)
if(${OpenCL_VERSION_MAJOR} EQUAL 1 AND ${OpenCL_VERSION_MINOR} LESS_EQUAL 2)
    include_directories("${OpenCL_INCLUDE_DIR}")
else()
    include_directories("${PROJECT_SOURCE_DIR}/src/OpenCL/include")
endif()

# Include project dependencies
# include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/include")


# Source file to library
add_library(FX3D SHARED "${PROJECT_SOURCE_DIR}/src/graphics.cpp"
                        "${PROJECT_SOURCE_DIR}/src/info.cpp"
                        "${PROJECT_SOURCE_DIR}/src/kernel.cpp"
                        "${PROJECT_SOURCE_DIR}/src/lbm.cpp"
                        "${PROJECT_SOURCE_DIR}/src/lodepng.cpp"
                        "${PROJECT_SOURCE_DIR}/src/shapes.cpp"
                        "${PROJECT_SOURCE_DIR}/src/fx3d/settings.cpp"
                        "${PROJECT_SOURCE_DIR}/src/fx3d/gsettings.cpp"
                        "${PROJECT_SOURCE_DIR}/src/fx3d/scenes.cpp")
target_link_libraries(FX3D OpenCL::OpenCL)
set_property(TARGET FX3D PROPERTY CXX_STANDARD 17) 



# Executable
add_executable(FluidX3D "${PROJECT_SOURCE_DIR}/src/fluidx.cpp"
                        "${PROJECT_SOURCE_DIR}/src/graphics.cpp"
                        "${PROJECT_SOURCE_DIR}/src/info.cpp")
target_link_libraries(FluidX3D FX3D OpenCL::OpenCL)
set_property(TARGET FluidX3D PROPERTY CXX_STANDARD 17) 

# Copy the skybox to the exe directory
add_custom_command(
        TARGET FluidX3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${PROJECT_SOURCE_DIR}/skybox/skybox8k.png
                ${CMAKE_SOURCE_DIR}/bin/skybox/skybox8k.png)
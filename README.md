# FluidX3D++
This is an attempt to make the repository (https://github.com/ProjectPhysX/FluidX3D)[FluidX3D] more flexible and
easily usable, removing some of the pre-processor directives and making them dynamically tunable parameters that
can be edited at runtime.  

Furthermore, we reworked the content to make the building process easily portable and configurable using CMake,
making FluidX3D an actual shared library, instead of a project that must be recompiled at every edit of a scene.  

The library is extended to be integrated with JSON configuration files, so that scenes and runs can be edited
offline and simulated without recompiling the project.  

For the documentation of the library we remand to the original repository.


## Build
The building process relies entirely on CMake.  
A working installation of OpenCL is needed for the project to be compiled successfully.  

Create a build directory and configure the CMake project
```sh
    mkdir build
    cd build
    cmake .. -A x64
```

Build the library in release mode for efficiency.
```sh
    cmake --build . --config release
```

The building process produces a `bin/` folder in the project's root directory. Inside the `bin/` folder the shared
library object `FX3D` is generated, together with a sample application `FluidX3D`.


## Sample application
The sample application can be executed with the following command
```sh
    bin/FluidX3D path/to/config.json
```
and it will run a simulation exporting the final frame inside `bin/export/t/`.  

The JSON configuration file contains two attributes:
 - `scene`: a string determining the scene to be simulated. The currently supported scenes are:
   - `"dam_break"`
   - `"colliding_particles"`
 - `num_frames`: the number of frames to simulate before exporting the rendering. Default value is `5000`.


## Adding new scenes
Suppose you want to add a new scene, which you want to call `MyScene`. First open the file `/include/fx3d/scenes.hpp`
and add the signature for the function
```c++
    fx3d::LBM* MySceneInit(const nlohmann::json& json);
```

Then, open the file `/src/fx3d/scenes.cpp` and implement the function `fx3d::MySceneInit()`.
It is important that you configure the project's settings **BEFORE** you do anything else, so every call to the
methods of `fx3d::Settings` and `fx3d::GraphicsSettings` must be done at the very beginning of the function.  
All the settings that in the original library are changed with pre-processor directives are now handled using
static methods from the two classes mentioned above. In particular, refer to the methods
```c++
    fx3d::Settings::EnableFeature()
    fx3d::Settings::SetVelocitySet()
    fx3d::Settings::SetCollisionType()
```


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


## Scenes usage

### Basics
The `fx3d::Scene` class is the main API object, and it is already usable as-is. 
It can be configured by giving as input to the constructor a json file such as the one you can find in `examples/config.json`. 

> Important: `config.export.out_dir` **must** end with path separator (`\\` or `/` depending on your platform) and **must not** have dots in it. This is due to the implementation of the save functions in FluidX3D. If you don't do this, arbitrarily bad things might happen - but most likely, you just won't find your output. You've been warned

The aspects of Scenes you can configure without writing a single line of code are the following:
* A set of simulation parameters, generally all the ones implemented for FluidX3D are available
* Disposition of solid, static obstacles in the scene, with mesh-specified geometry
* Disposition of dynamic fluid bodies in the scene, with mesh-specified geometry

Some important caveats: 
* Scales and translations are defined with integer units. While the values are still interpreted as floating point, the embedding space of the simulation domain is the discretized real interval `[0; N_i-1]`, where `N_i` is the specified resolution for the `i`-th dimension of the domain
* Meshes have to be in STL format. Everything else won't be loaded or will throw exceptions
* The `size` attribute is used when loading the mesh, and it indicates the lenght of the longest side of the mesh's bounding box after rescaling. On top of this, you may further scale the mesh to your liking using the `scale` attribute.

### What if this is not enough?

`fx3d::Scene` is designed to be extended with subclasses. The following features can be reimplemented:
1. `fx3d::Scene::config_sim_params` -- interface to copy simulation parameters from config
2. `fx3d::Scene::config_obstacles` -- interface to load obstacles geometry into the scene
3. `fx3d::Scene::config_fluid_bodies` -- interface to load the initial state of fluid bodies into the scene
4. `fx3d::Scene::config_export` -- interface to configure export settings
5. `fx3d::Scene::config_graphics` -- interface to configure graphics settings, e.g., rendering mode (see `defines.hpp`) or camera positioning using the `Camera` object provided by FluidX3D
6. `fx3d::Scene::is_fluid` -- describes which cells are fluid in the initial state given the `x,y,z` coordinates
7. `fx3d::Scene::is_static` -- describes which cells are solid obstacles given the `x,y,z` coordinates
8. `fx3d::Scene::is_boundary` -- describes which cells are domain boundary given the `x,y,z` coordinates
8. `fx3d::Scene::export_frame` -- procedure to export the contents of the current frame (for base `Scene`s, it renders the current frame from a single view)
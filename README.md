# Description
A sumobot simulator for evaluating different control strategies.

Work in progress. You can follow the progress here:
https://www.artfulbytes.com/sumo-simulator-journal

# Cloning

# Build
## Linux
1. mkdir build
2. cd build
3. cmake ..
4. make

## Windows

# Folder structure
* include
    - physics/
        + Include files for the physics part of the simulator
    - renderer/
        + Include files for the renderer part of the simulator
* src
    - physics
        + Implementation files for the physics part of the simulator
        + Basically wrappers around Box2D
    - renderer
        + Implementation files for the renderer part of the simulator
        + Boilerplate around OpenGL, GLAD, ImGui
* external
    - All external dependencies/libraries as git modules
* resources
    - Resources for the renderer (e.g. shaders, textures, sprites..)
* tools
    - Any standalone tools (e.g. scripts)

# Troubleshooting...
* Incorrect OpenGL version
    - Check version in console output
    - Try exporting MESA_GL_VERSION_OVERRIDE=3.3



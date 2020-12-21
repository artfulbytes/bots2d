# Description
A sumobot simulator for evaluating different control strategies.

Work in progress. You can follow the progress here:
https://www.artfulbytes.com/sumo-simulator-journal

# Cloning
The project depends on other git projects checked out at specific commits.
All external projects are included as submodules under "external/". To clone
the repo including the submodules run:

git clone --recursive -j8 https://github.com/artfulbytes/simulator

# Build
## Linux
1. mkdir build
2. cd build
3. cmake ..
4. make

## Windows

# Troubleshooting...
* Incorrect OpenGL version (Linux)
    - Check version in console output
    - "export MESA_GL_VERSION_OVERRIDE=3.3"
* "RandR headers not found; install libxrandr development package"
    - GLFW requires xorg-dev liblu1-mesa-dev
    - On Ubuntu, run "sudo apt install xorg-dev libglu1-mesa-dev"

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



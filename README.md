[![Win Build](https://github.com/Zly-u/OpenGL_Learnings/actions/workflows/cmake-windows-build.yml/badge.svg)](https://github.com/Zly-u/OpenGL_Learnings/actions/workflows/cmake-windows-build.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/b4b3993d20714510829ce51d808f099f)](https://app.codacy.com/gh/Zly-u/OpenGL_Learnings/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
# 2D OpenGL Learning area.

I'm just learning OpenGL in here and experimenting.

This is not meant to be anything for anyone to use, but maybe you can find some useful techniques and code examples in here, idk.

CW: Trying out a bunch of meta-programming shenanigans in here.

## Key notes about the project
* Custom self made delegates for handling callbacks/events.
* Templated way of handling `Shader Programs`, `Uniform Parameters` in Shaders and `Vertex Attributes` for polygons, because I hate how it has no error handling internally.
* Flexible way of setting up how exactly your polygons are rendered.
* Following principles of SOLID where applicable.
* Trying to use as much C++17-23 as possible.

## How to compile

The project doesn't require any setups aside from [VCPKG](https://vcpkg.io/en/) being installed.

So... Just run the CMake build process the usual way you do, either via CLion, VS, VSC, or terminal.

And hopefully it works first try, I tried my best to make it as portable as possible.

At Post-Compilation - all assets are moved into the same folder where executable is, everything is automatic.

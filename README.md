# GLFragmentShader

Simple full-window GL fragment shader using GLFW and glad.

## Compiling

Clone the repository with `git clone --recursive <repo url>` to ensure that the GLFW submodule is initialized. Building and compiling *should* be painless using CMake.

Build tested with CMake 3.27.3 and Visual Studio 2022 on Windows.

## Dependencies

Building requires CMake 3.5 or later. All dependencies are built into the repository, so no additional work is needed to include them aside from ensuring that git submodules are initialized.

The following dependencies are included in the `deps` folder:

- [GLFW](https://www.glfw.org/) windowing toolkit and GL context manager
- [Glad](https://github.com/Dav1dde/glad) GL loader code generated using the [glad2 webservice](https://gen.glad.sh/)

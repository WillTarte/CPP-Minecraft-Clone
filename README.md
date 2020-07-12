# COMP-371-Proj

## Royal Octopi Team Members
 - Zahra Nikbakht 40138253
 - Philip Dumaresq 40082638
 - William Tarte 40087900
 - Mohammad Ali Zahir 40077619
 - Ewan McNeil 40021787
 
## Library Versions
 - glfw-3.3.2
 - glew-2.1.0
 - glm-0.9.9.8
 - Minimum OpengGL version = 3.3
 
## Installation Guide
_This project has been successfully compiled on both Arch Linux Manjaro using GCC  
and on Windows 10 using Microsoft Visual Studio 2017's compiler (MSVC)_
  
_The IDE that has been tested is CLION 2019/2020. On Windows, the toolchain had to manually be changed to use MSVC._
_It should also work with Visual Studio with CMake support._

1. Either clone the repository or download the zip file and unzip it.
2. cd into the project folder and run cmake to build the project.
3. Once the build successfully finishes, you can run the executable in the build folder that was generated.

## Controls
 - Numpad 1 to 6 selects the currently controlled model.  
    - 1 to 5 selects one of the student models.
    - 6 selects the world.
 - Left-Shift + (W,A,S,D) moves the currently selected model up, left, down, right respectively.
 - A/D rotates the currently selected model on the Y axis.
 - U/J scales the currently selected model up/down respectively.
 - __With the world selected,__ the arrow keys rotate the world on the Y and X axis.
 - The _Home_ button resets the world/models to their original position/orientation.
 - The mouse is TODO

## Resources
 - https://learnopengl.com/
 - https://en.cppreference.com/w/
 - https://www.khronos.org/registry/OpenGL-Refpages/gl4/

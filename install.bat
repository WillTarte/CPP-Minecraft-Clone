@ECHO OFF
ECHO Building COMP-371-Project ...
mkdir "build"
cmake -S . -B ".\build"
ECHO Generating executable ...
cd ".\build"
cmake --build . --target "COMP-371-Project"
cd ".\Debug"
copy "COMP-371-Project.exe" "..\."
ECHO Executable generated successfully.
PAUSE
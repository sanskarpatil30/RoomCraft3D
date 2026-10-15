@echo off
setlocal

echo ===================================================
echo   Building RoomCraft 3D (C++ OpenGL Mini Project)
echo ===================================================

set CXX="C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe"
if not exist %CXX% (
    set CXX=g++
)

echo Using Compiler: %CXX%

if not exist bin mkdir bin

echo Compiling all C++ source files...
%CXX% -O2 -std=c++11 -Wall ^
    src/main.cpp ^
    src/camera.cpp ^
    src/room.cpp ^
    src/furniture.cpp ^
    src/scene.cpp ^
    src/renderer.cpp ^
    src/input.cpp ^
    src/save_load.cpp ^
    -Iinclude ^
    -Iexternal/freeglut/include ^
    -Lexternal/freeglut/lib ^
    -lfreeglut -lopengl32 -lglu32 ^
    -o bin/RoomCraft3D.exe

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERROR] Build failed! Check compiler output above.
    exit /b %ERRORLEVEL%
)

echo Copying FreeGLUT DLL to bin directory...
copy /Y external\freeglut\bin\libfreeglut.dll bin\libfreeglut.dll >nul

if not exist layouts mkdir layouts
if not exist bin\layouts mkdir bin\layouts
copy /Y layouts\sample_room.txt bin\layouts\sample_room.txt >nul 2>&1

echo.
echo ===================================================
echo   BUILD SUCCESSFUL!
echo   Executable generated at: bin\RoomCraft3D.exe
echo ===================================================

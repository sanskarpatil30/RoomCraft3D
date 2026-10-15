@echo off
setlocal
cd /d "%~dp0"

echo ========================================================
echo   Launching RoomCraft 3D - Interactive Interior Designer
echo ========================================================

if exist bin\RoomCraft3D.exe (
    cd bin
    start "" RoomCraft3D.exe
) else (
    echo [ERROR] bin\RoomCraft3D.exe not found!
    echo Please run build.bat first to compile the project.
    pause
)

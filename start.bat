@echo off
REM ArduinoMaze Quick Start Script for Windows
REM This script starts the server and opens Firefox

echo.
echo ========================================
echo   ArduinoMaze - Arduino Maze Game
echo ========================================
echo.

REM Check if Node.js is installed
where node >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: Node.js is not installed!
    echo Please install Node.js from https://nodejs.org/
    pause
    exit /b 1
)

REM Check if npm is installed
where npm >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: npm is not installed!
    echo Please install npm (comes with Node.js)
    pause
    exit /b 1
)

REM Get the directory where this script is located
cd /d "%~dp0"

REM Check if node_modules exists
if not exist "node_modules" (
    echo Installing dependencies...
    call npm install
    echo.
)

REM Start the server
echo Starting Node.js server...
start "ArduinoMaze Server" cmd /k node server.js

REM Wait a bit for server to start
timeout /t 2 /nobreak

REM Open Firefox
echo Opening Firefox...
timeout /t 1 /nobreak
start firefox http://localhost:3000

echo.
echo ArduinoMaze is ready!
echo Server URL: http://localhost:3000
echo.
echo Close this window or press Ctrl+C to stop the server.
pause

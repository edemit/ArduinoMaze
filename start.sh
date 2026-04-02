#!/bin/bash

# ArduinoMaze Quick Start Script
# This script starts the server and opens Firefox

echo "🚀 Starting ArduinoMaze Server..."
echo ""

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo "❌ Node.js is not installed!"
    echo "   Please install Node.js from https://nodejs.org/"
    exit 1
fi

# Check if npm is installed
if ! command -v npm &> /dev/null; then
    echo "❌ npm is not installed!"
    echo "   Please install npm (comes with Node.js)"
    exit 1
fi

# Get the directory where this script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

# Check if node_modules exists
if [ ! -d "node_modules" ]; then
    echo "📦 Installing dependencies..."
    npm install
    echo ""
fi

# Start the server in background
echo "⚙️  Starting Node.js server..."
node server.js &
SERVER_PID=$!

# Wait for server to start
sleep 2

# Check if server is running
if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo "❌ Server failed to start!"
    exit 1
fi

echo "✓ Server is running (PID: $SERVER_PID)"
echo ""
echo "🌐 Opening Firefox..."
sleep 1

# Try to open Firefox with the localhost URL
if command -v firefox &> /dev/null; then
    firefox http://localhost:3000 &
elif command -v firefox.exe &> /dev/null; then
    firefox.exe http://localhost:3000 &
else
    echo "⚠️  Firefox not found! Please open http://localhost:3000 manually in your browser"
fi

echo ""
echo "✓ ArduinoMaze is ready!"
echo ""
echo "📡 Server URL: http://localhost:3000"
echo "🔌 Press Ctrl+C to stop the server"
echo ""

# Keep the server running
wait $SERVER_PID

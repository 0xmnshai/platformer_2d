#!/bin/bash

# Build script for platformer_2d project

set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
SRC_DIR="$PROJECT_DIR/src"

# check if there is build directory then delete it
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"

# Navigate to build directory
cd "$BUILD_DIR"

# Configure with CMake
cmake ..

# Build the project
make

echo "Build completed successfully!"
echo "Output: $BUILD_DIR"

# running the game
"$BUILD_DIR/platformer_2d"
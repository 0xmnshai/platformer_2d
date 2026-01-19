#!/bin/bash


set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
 
CMAKE_FLAGS=""
EXECUTABLE="platformer_2d"

if [[ "$1" == "-t" || "$1" == "test" ]]; then
    echo "Building Tile Test only"
    CMAKE_FLAGS="-DTILE_TEST=ON"
    EXECUTABLE="tile_test"
else
    echo "Building Full Game"
fi
 
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
 
cmake .. $CMAKE_FLAGS
cmake --build .

echo "Build completed successfully!"
echo "Output: $BUILD_DIR"
 
if [ -f "$BUILD_DIR/$EXECUTABLE" ]; then
    echo "Running $EXECUTABLE"
    "$BUILD_DIR/$EXECUTABLE"
else
    echo "Executable not found: $EXECUTABLE"
    exit 1
fi

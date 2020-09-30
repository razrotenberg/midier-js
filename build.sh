#!/bin/bash

set -e

if [ ! -d /Midier ]; then
    echo "Cloning Midier repository"
    git clone https://github.com/levosos/Midier.git /Midier
fi

mkdir -p /MidierJS/dist

echo "Building with Emscripten"
emcc \
    -Os \
    -s STRICT=1 \
    -s WASM=1 \
    -s SINGLE_FILE=1 \
    -s ENVIRONMENT='web' \
    -s EXPORT_ES6=1 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME='createMidier' \
    -s ALLOW_TABLE_GROWTH=1 \
    -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall","cwrap","addFunction","removeFunction"]' \
    -s FILESYSTEM=0 \
    -I /Midier/src/ \
    -o /MidierJS/dist/Midier.js \
    --pre-js /MidierJS/src/js/pre.js \
    /Midier/src/*/*.cpp /MidierJS/src/*/*.cpp

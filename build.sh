#!/bin/bash

set -e

if [ ! -d /midier-core ]; then
    echo "Cloning Midier repository"
    git clone https://github.com/levosos/midier-core.git /midier-core
fi

mkdir -p /midier-js/dist

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
    -I /midier-core/src/ \
    -o /midier-js/dist/Midier.js \
    --pre-js /midier-js/src/js/pre.js \
    /midier-core/src/*/*.cpp /midier-js/src/*/*.cpp

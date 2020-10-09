#!/bin/bash

docker run -it --rm -v $(pwd):/midier-js -w /midier-js "$@" emscripten/emsdk

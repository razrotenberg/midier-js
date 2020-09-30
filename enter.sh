#!/bin/bash

docker run -it --rm -v $(pwd):/MidierJS -w /MidierJS "$@" emscripten/emsdk

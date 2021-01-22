load(":emscripten_wasm_js.bzl", "emscripten_wasm_js")

cc_library(
    name = "lib",
    srcs = ["cpp/main.cpp"],
    deps = ["@midier-core//src:lib"],
)

emscripten_wasm_js(
    name = "midier.js",
    libs = [
        ":lib",
        "@midier-core//src:lib",
    ],
    opts = [
        "STRICT=1",
        "WASM=1",
        "SINGLE_FILE=1",
        "ENVIRONMENT='web'",
        "EXPORT_ES6=1",
        "MODULARIZE=1",
        "EXPORT_NAME='createMidier'",
        "ALLOW_TABLE_GROWTH=1",
        "FILESYSTEM=0",
    ],
    prejs = "js/pre.js",
    methods = ['ccall', 'cwrap', 'addFunction', 'removeFunction'],
)

load(":emscripten_wasm_js.bzl", "emscripten_wasm_js")

emscripten_wasm_js(
    name = "midier.js",
    libs = [
        "//src/cpp:main",
        "//src/cpp/lib:lib",
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
    prejs = "pre.js",
    methods = ['ccall', 'cwrap', 'addFunction', 'removeFunction'],
)

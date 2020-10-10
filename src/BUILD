load(":emscripten_wasm_js.bzl", "emscripten_wasm_js")

emscripten_wasm_js(
    name = "midier.js",
    srcs = ["cpp/main.cpp"],
    opts = [
        "STRICT=1",
        "WASM=1",
        "SINGLE_FILE=1",
        "ENVIRONMENT='web'",
        "EXPORT_ES6=1",
        "MODULARIZE=1",
        "EXPORT_NAME='createMidier'",
        "ALLOW_TABLE_GROWTH=1",
        "EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\",\"addFunction\",\"removeFunction\"]'",
        "FILESYSTEM=0",
    ],
    prejs = "src/js/pre.js", # NOTE: changes in the JavaScript will not cause Bazel to ignore the cache. run `bazel clean` to fix this.
    deps = ["@midier-core//src:lib"],
)

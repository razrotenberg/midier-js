EMSCRIPTEN_OPTS = [
    "-s STRICT=1",
    "-s WASM=1",
    "-s SINGLE_FILE=1",
    "-s ENVIRONMENT='web'",
    "-s EXPORT_ES6=1",
    "-s MODULARIZE=1",
    "-s EXPORT_NAME='createMidier'",
    "-s ALLOW_TABLE_GROWTH=1",
    "-s EXTRA_EXPORTED_RUNTIME_METHODS='[\"ccall\",\"cwrap\",\"addFunction\",\"removeFunction\"]'",
    "-s FILESYSTEM=0",

    # add the pre-js JavaScript code to the result module
    "--pre-js src/js/pre.js",
]

cc_binary(
    name = "midier.js",
    srcs = ["cpp/main.cpp"],
    linkopts = EMSCRIPTEN_OPTS,
    deps = [
        "@midier-core//src:lib",
    ],
)

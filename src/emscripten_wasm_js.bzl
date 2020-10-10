def emscripten_wasm_js(opts=[], prejs=None, linkopts=[], data=[], **kwargs):
    linkopts = linkopts + ['-s %s' % opt for opt in opts]

    if prejs != None:
        linkopts = linkopts + ['--pre-js %s' % prejs]

    native.cc_binary(
        linkopts = linkopts,
        **kwargs)

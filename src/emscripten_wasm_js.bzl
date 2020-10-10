def emscripten_wasm_js(name, libs, opts=[], methods=None, prejs=None):
    if methods:
        opts = opts + ["EXTRA_EXPORTED_RUNTIME_METHODS=[%s]" % ",".join(["'%s'" % method for method in methods])]

    components = ["emcc -o $@ "]

    if prejs:
        components = components + ["--pre-js $(location %s)" % prejs]

    components = components + ["-s %s" % opt for opt in opts]
    components = components + ["$(location %s)" % lib for lib in libs]

    native.genrule(
        name = "_" + name,
        srcs = [prejs] + libs,
        outs = [name],
        cmd = " ".join(components),
    )

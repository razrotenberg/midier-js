load("@bazel_tools//tools/build_defs/cc:action_names.bzl", "ACTION_NAMES")
load(
    "@bazel_tools//tools/cpp:cc_toolchain_config_lib.bzl",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
    "with_feature_set",
)

def _impl(ctx):
    tool_paths = [
        tool_path(
            name = "gcc",
            path = "/emsdk/upstream/emscripten/emcc",
        ),
        tool_path(
            name = "ld",
            path = "/emsdk/upstream/emscripten/emcc",
        ),
        tool_path(
            name = "ar",
            path = "/emsdk/upstream/emscripten/emar",
        ),
        tool_path(
            name = "cpp",
            path = "/bin/false",
        ),
        tool_path(
            name = "gcov",
            path = "/bin/false",
        ),
        tool_path(
            name = "nm",
            path = "/bin/false",
        ),
        tool_path(
            name = "objdump",
            path = "/bin/false",
        ),
        tool_path(
            name = "strip",
            path = "/bin/false",
        ),
    ]

    preprocessor_compile_actions = [
        ACTION_NAMES.c_compile,
        ACTION_NAMES.cpp_compile,
        ACTION_NAMES.linkstamp_compile,
        ACTION_NAMES.preprocess_assemble,
        ACTION_NAMES.cpp_header_parsing,
        ACTION_NAMES.cpp_module_compile,
        ACTION_NAMES.clif_match,
    ]

    all_link_actions = [
        ACTION_NAMES.cpp_link_executable,
        ACTION_NAMES.cpp_link_dynamic_library,
        ACTION_NAMES.cpp_link_nodeps_dynamic_library,
    ]

    all_compile_actions = [
        ACTION_NAMES.c_compile,
        ACTION_NAMES.cpp_compile,
        ACTION_NAMES.linkstamp_compile,
        ACTION_NAMES.assemble,
        ACTION_NAMES.preprocess_assemble,
        ACTION_NAMES.cpp_header_parsing,
        ACTION_NAMES.cpp_module_compile,
        ACTION_NAMES.cpp_module_codegen,
        ACTION_NAMES.clif_match,
        ACTION_NAMES.lto_backend,
    ]

    toolchain_include_directories_feature = feature(
        name = "toolchain_include_directories",
        enabled = True,
        flag_sets = [
            flag_set(
                actions = all_compile_actions,
                flag_groups = [
                    flag_group(
                        flags = [
                            # bazel creates a symbolic link at "external/emsdk" to the "emsdk" repository
                            # and inside it there's a symlink at "emsdk" to /emsdk
                            "-isystem",
                            "external/emsdk/emsdk/upstream/emscripten/system/include/libcxx",
                            "-isystem",
                            "external/emsdk/emsdk/upstream/emscripten/system/lib/libcxxabi/include",
                            "-isystem",
                            "external/emsdk/emsdk/upstream/emscripten/system/include/compat",
                            "-isystem",
                            "external/emsdk/emsdk/upstream/emscripten/system/include",
                            "-isystem",
                            "external/emsdk/emsdk/upstream/emscripten/system/include/libc",
                            "-isystem",
                            "external/emsdk/emsdk/upstream/emscripten/system/lib/libc/musl/arch/emscripten",
                            "-isystem",
                            "external/emsdk/emsdk/upstream/emscripten/system/local/include",
                        ],
                    ),
                ],
            ),
        ],
    )

    crosstool_default_flag_sets = [
        # Optimized (opt) - optimizing for the web (code size)
        flag_set(
            actions = all_compile_actions + all_link_actions,
            flag_groups = [flag_group(flags = ["-g0", "-Os"])],
            with_features = [with_feature_set(features = ["opt"])],
        ),
        # Fastbuild (fastbuild)
        flag_set(
            actions = all_compile_actions + all_link_actions,
            flag_groups = [flag_group(flags = ["-O2"])],
            with_features = [with_feature_set(features = ["fastbuild"])],
        ),
        # Debug (dbg)
        flag_set(
            actions = preprocessor_compile_actions,
            flag_groups = [flag_group(flags = ["-DDEBUG"])],
            with_features = [with_feature_set(features = ["dbg"])],
        ),
        flag_set(
            actions = all_compile_actions + all_link_actions,
            flag_groups = [flag_group(flags = ["-g2", "-O0"])],
            with_features = [with_feature_set(features = ["dbg"])],
        ),
    ]

    features = [
        toolchain_include_directories_feature,
        feature(
            name = "opt",
            provides = ["variant:crosstool_build_mode"],
        ),
        feature(
            name = "dbg",
            provides = ["variant:crosstool_build_mode"],
        ),
        feature(
            name = "fastbuild",
            provides = ["variant:crosstool_build_mode"],
        ),
        feature(
            name = "crosstool_default_flags",
            enabled = True,
            flag_sets = crosstool_default_flag_sets,
        ),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        toolchain_identifier = "wasm-toolchain",
        host_system_name = "", # "i686-unknown-linux-gnu",
        target_system_name = "", # "wasm-unknown-emscripten",
        target_cpu = "wasm",
        target_libc = "",
        compiler = "emscripten",
        abi_version = "",
        abi_libc_version = "",
        tool_paths = tool_paths,
        features = features,
    )

cc_toolchain_config = rule(
    implementation = _impl,
    attrs = {},
    provides = [CcToolchainConfigInfo],
)

def _emsdk_impl(ctx):
    if "EMSDK" not in ctx.os.environ or ctx.os.environ["EMSDK"].strip() == "":
        fail("The environment variable EMSDK is not found. " +
             "Did you run source ./emsdk_env.sh ?" +
             "Are you running from the docker image levosos/midier-js ?")
    path = ctx.os.environ["EMSDK"]
    ctx.symlink(path, "emsdk")
    ctx.file("BUILD", """
filegroup(
    name = "all",
    srcs = glob(["emsdk/**"]),
    visibility = ["//visibility:public"],
)
""")

emsdk_configure = repository_rule(
    implementation = _emsdk_impl,
    local = True,
)

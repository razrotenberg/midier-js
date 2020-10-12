FROM emscripten/emsdk:2.0.6

RUN wget -O bazel.sh https://github.com/bazelbuild/bazel/releases/download/3.6.0/bazel-3.6.0-installer-linux-x86_64.sh && \
    chmod +x bazel.sh && \
    ./bazel.sh && \
    rm bazel.sh

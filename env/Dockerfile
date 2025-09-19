FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    python3-pip \
    git \
    && pip3 install --no-cache-dir conan \
    && rm -rf /var/lib/apt/lists/*

RUN conan profile detect

WORKDIR /workspace

COPY . /workspace

CMD bash -lc "\
    mkdir -p build && cd build && \
    conan install .. \
    --install-folder . \
    --build=missing \
    -g CMakeToolchain \
    -g CMakeDeps && \
    cmake \
    -S .. -B . \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake && \
    cmake --build . && \
    ninja test \
    "



FROM --platform=linux/amd64 ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential scons git python3 clang-format-14 valgrind \
    && ln -s /usr/bin/clang-format-14 /usr/bin/clang-format \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace
FROM --platform=linux/amd64 ubuntu:22.04

RUN apt-get update && apt-get install -y \
    gcc g++ \
    clang-14 clang++-14 \
    cmake make valgrind clang-format-14

RUN ln -s /usr/bin/clang-format-14 /usr/bin/clang-format
RUN ln -s /usr/bin/clang-14 /usr/bin/clang
RUN ln -s /usr/bin/clang++-14 /usr/bin/clang++
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace

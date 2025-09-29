FROM --platform=linux/amd64 debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    gcc g++ \
    clang \
    cmake make \
    valgrind \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . /workspace

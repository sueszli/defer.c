# 
# containerized development
# 

DOCKER_RUN = docker run --rm -v $(PWD):/workspace main sh -c

.PHONY: build-image
build-image:
	docker build -t main .

.PHONY: docker-gcc
docker-gcc:
	$(DOCKER_RUN) "mkdir -p /tmp/gcc-build && cd /tmp/gcc-build && cmake -DCMAKE_C_COMPILER=gcc /workspace && cmake --build . -j$$(nproc) && ./defer"

.PHONY: docker-clang
docker-clang:
	$(DOCKER_RUN) "mkdir -p /tmp/clang-build && cd /tmp/clang-build && cmake -DCMAKE_C_COMPILER=clang /workspace && cmake --build . -j$$(nproc) && ./defer"

.PHONY: docker-valgrind
docker-valgrind:
	$(DOCKER_RUN) "mkdir -p /tmp/valgrind-build && cd /tmp/valgrind-build && cmake -DCMAKE_C_COMPILER=gcc -DDISABLE_ASAN=ON /workspace && cmake --build . -j$$(nproc) && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./defer"

.PHONY: docker-test
docker-test:
	$(DOCKER_RUN) "mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DCMAKE_C_COMPILER=gcc -DBUILD_TESTS=ON /workspace && cmake --build . -j$$(nproc) && ctest --output-on-failure"

.PHONY: docker-clean
docker-clean:
	docker rmi main

# 
# apple silicon development
# 

.PHONY: run
run:
	mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_C_COMPILER=clang $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && cmake --build . --target run

# also try: Instruments
.PHONY: leaks
leaks:
	mkdir -p /tmp/leaks-build && cd /tmp/leaks-build && cmake -DCMAKE_C_COMPILER=clang -DDISABLE_ASAN=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
	codesign -s - -f --entitlements entitlements.plist /tmp/leaks-build/defer
	MallocStackLogging=1 leaks --atExit -- /tmp/leaks-build/defer

.PHONY: test
test:
	mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DCMAKE_C_COMPILER=clang -DBUILD_TESTS=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && ctest --output-on-failure

# 
# utils
# 

.PHONY: fmt
fmt:
	uvx --from cmakelang cmake-format --dangle-parens --line-width 500 -i CMakeLists.txt
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i

# 
# docker container
# 

DOCKER_RUN = docker run --rm -v $(PWD):/workspace main sh -c

.PHONY: build-image
build-image:
	docker build -t main .
 
.PHONY: docker-gcc
docker-gcc:
	$(DOCKER_RUN) "mkdir -p /tmp/gcc-build && cd /tmp/gcc-build && cmake -DCMAKE_C_COMPILER=gcc /workspace && cmake --build . -j$$(nproc) && ./binary"

.PHONY: docker-clang
docker-clang:
	$(DOCKER_RUN) "mkdir -p /tmp/clang-build && cd /tmp/clang-build && cmake -DCMAKE_C_COMPILER=clang /workspace && cmake --build . -j$$(nproc) && ./binary"

.PHONY: docker-valgrind
docker-valgrind:
	$(DOCKER_RUN) "mkdir -p /tmp/valgrind-build && cd /tmp/valgrind-build && cmake -DCMAKE_C_COMPILER=gcc -DDISABLE_ASAN=ON /workspace && cmake --build . -j$$(nproc) && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./binary"

.PHONY: docker-test
docker-test:
	$(DOCKER_RUN) "mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DCMAKE_C_COMPILER=gcc -DBUILD_TESTS=ON /workspace && cmake --build . -j$$(nproc) && ctest --output-on-failure"

.PHONY: docker-clean
docker-clean:
	docker rmi main

# 
# apple silicon
# 

.PHONY: run
run: fmt lint
	mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_C_COMPILER=clang $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && ASAN_OPTIONS=detect_leaks=1 ./binary

.PHONY: run-gmalloc
run-gmalloc:
	mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_C_COMPILER=clang -DDISABLE_ASAN=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && MALLOC_PROTECT_BEFORE=1 MallocStackLogging=1 MallocScribble=1 MallocPreScribble=1 MallocErrorAbort=1 DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib ./binary

.PHONY: leaks
leaks:
	mkdir -p /tmp/leaks-build && cd /tmp/leaks-build && cmake -DCMAKE_C_COMPILER=clang -DDISABLE_ASAN=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
	codesign -s - -f --entitlements entitlements.plist /tmp/leaks-build/binary
	leaks --atExit --list --groupByType -- /tmp/leaks-build/binary

.PHONY: test
test:
	mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DCMAKE_C_COMPILER=clang -DBUILD_TESTS=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && ctest --output-on-failure

# 
# utils
# 

.PHONY: lint
lint:
	@cppcheck --enable=all --std=c23 --language=c --suppress=missingIncludeSystem --suppress=checkersReport --check-level=exhaustive --inconclusive -I src/ src/

.PHONY: fmt
fmt:
	@uvx --from cmakelang cmake-format --dangle-parens --line-width 500 -i CMakeLists.txt
	@find . -name "*.c" -o -name "*.h" | xargs clang-format -i

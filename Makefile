DOCKER_RUN = docker run --rm -v $(PWD):/workspace main sh -c

.PHONY: build-image
build-image:
	docker build -t main .

.PHONY: run-gcc
run-gcc: build-image
	$(DOCKER_RUN) "mkdir -p /tmp/gcc-build && cd /tmp/gcc-build && cmake -DCMAKE_C_COMPILER=gcc /workspace && make && ./defer"

.PHONY: run-clang
run-clang: build-image
	$(DOCKER_RUN) "mkdir -p /tmp/clang-build && cd /tmp/clang-build && cmake -DCMAKE_C_COMPILER=clang /workspace && make && ./defer"

.PHONY: run-valgrind
run-valgrind: build-image
	$(DOCKER_RUN) "mkdir -p /tmp/valgrind-build && cd /tmp/valgrind-build && cmake -DDISABLE_ASAN=ON /workspace && make && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./defer"

.PHONY: run-local
run-local:
	cd $$(mktemp -d) && cmake -DCMAKE_C_COMPILER=clang $(PWD) && make && ./defer

.PHONY: run-local-leaks
run-local-leaks:
	cd $$(mktemp -d) && cmake -DDISABLE_ASAN=ON $(PWD) && make && leaks --atExit -- ./defer

.PHONY: test
test: build-image
	$(DOCKER_RUN) "mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DBUILD_TESTS=ON /workspace && make && ctest --output-on-failure"

.PHONY: test-local
test-local:
	cd $$(mktemp -d) && cmake -DBUILD_TESTS=ON $(PWD) && make && ctest --output-on-failure

.PHONY: fmt
fmt:
	uvx --from cmakelang cmake-format --dangle-parens --line-width 120 -i CMakeLists.txt
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i

.PHONY: clean
clean:
	docker rmi main

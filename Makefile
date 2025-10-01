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
	$(DOCKER_RUN) "mkdir -p /tmp/valgrind-build && cd /tmp/valgrind-build && cmake -DDISABLE_ASAN=ON /workspace && cmake --build . -j$$(nproc) && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./defer"

.PHONY: docker-test
docker-test:
	$(DOCKER_RUN) "mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DBUILD_TESTS=ON /workspace && cmake --build . -j$$(nproc) && ctest --output-on-failure"

.PHONY: docker-clean
docker-clean:
	docker rmi main

# 
# apple silicon development
# 

.PHONY: run
run:
	mkdir -p /tmp/build && cd /tmp/build && cmake -DCMAKE_C_COMPILER=clang $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && ./defer

.PHONY: leaks
leaks:
	mkdir -p /tmp/leaks-build && cd /tmp/leaks-build && cmake -DCMAKE_C_COMPILER=clang $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
	codesign -s - -f --entitlements entitlements.plist /tmp/leaks-build/defer
	leaks --atExit -- /tmp/leaks-build/defer

# .PHONY: asan
# asan:
# 	mkdir -p /tmp/asan-build && cd /tmp/asan-build && cmake -DCMAKE_C_COMPILER=clang $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
# 	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 /tmp/asan-build/defer

# .PHONY: malloc-debug
# malloc-debug:
# 	mkdir -p /tmp/malloc-build && cd /tmp/malloc-build && cmake -DCMAKE_C_COMPILER=clang $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
# 	codesign -s - -f --entitlements entitlements.plist /tmp/malloc-build/defer
# 	MallocStackLogging=1 MallocScribble=1 MallocGuardEdges=1 MallocCheckHeapStart=1000 MallocCheckHeapEach=100 /tmp/malloc-build/defer

# .PHONY: ubsan
# ubsan:
# 	mkdir -p /tmp/ubsan-build && cd /tmp/ubsan-build && cmake -DCMAKE_C_COMPILER=clang -DDISABLE_UBSAN=OFF $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
# 	/tmp/ubsan-build/defer

# .PHONY: instruments
# instruments:
# 	mkdir -p /tmp/instruments-build && cd /tmp/instruments-build && cmake -DCMAKE_C_COMPILER=clang $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
# 	codesign -s - -f --entitlements entitlements.plist /tmp/instruments-build/defer
# 	@echo "Recording with Instruments (Leaks template)..."
# 	xctrace record --template 'Leaks' --output /tmp/defer-leaks.trace --launch -- /tmp/instruments-build/defer
# 	@echo "Trace saved to /tmp/defer-leaks.trace"
# 	@echo "Opening trace file..."
# 	open /tmp/defer-leaks.trace

# .PHONY: check-all-leaks
# check-all-leaks:
# 	@echo "=== Running ASan ==="
# 	@$(MAKE) asan || true
# 	@echo ""
# 	@echo "=== Running leaks tool ==="
# 	@$(MAKE) leaks || true
# 	@echo ""
# 	@echo "=== Running MallocDebug ==="
# 	@$(MAKE) malloc-debug || true
# 	@echo ""
# 	@echo "All leak checks complete!"

.PHONY: test
test:
	mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DCMAKE_C_COMPILER=clang -DBUILD_TESTS=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && ctest --output-on-failure

# 
# utils
# 

.PHONY: fmt
fmt:
	uvx --from cmakelang cmake-format --dangle-parens --line-width 120 -i CMakeLists.txt
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i

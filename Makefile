# 
# containerized development
# 

DOCKER_RUN = docker run --rm -v $(PWD):/workspace main sh -c

.PHONY: build-image
build-image:
	docker build -t main .

.PHONY: docker-gcc
docker-gcc: build-image
	$(DOCKER_RUN) "mkdir -p /tmp/gcc-build && cd /tmp/gcc-build && cmake -DCMAKE_C_COMPILER=gcc /workspace && cmake --build . -j$$(nproc) && ./defer"

.PHONY: docker-clang
docker-clang: build-image
	$(DOCKER_RUN) "mkdir -p /tmp/clang-build && cd /tmp/clang-build && cmake -DCMAKE_C_COMPILER=clang /workspace && cmake --build . -j$$(nproc) && ./defer"

.PHONY: docker-valgrind
docker-valgrind: build-image
	$(DOCKER_RUN) "mkdir -p /tmp/valgrind-build && cd /tmp/valgrind-build && cmake -DDISABLE_ASAN=ON /workspace && cmake --build . -j$$(nproc) && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./defer"

.PHONY: docker-test
docker-test: build-image
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
	mkdir -p /tmp/leaks-build && cd /tmp/leaks-build

	echo '<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd"><plist version="1.0"><dict><key>com.apple.security.get-task-allow</key><true/></dict></plist>' > entitlements.plist

	cmake -DDISABLE_ASAN=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu)
	
	codesign -s - -f --entitlements entitlements.plist ./defer

	leaks --atExit -- ./defer

.PHONY: test
test:
	mkdir -p /tmp/test-build && cd /tmp/test-build && cmake -DBUILD_TESTS=ON $(PWD) && cmake --build . -j$$(sysctl -n hw.ncpu) && ctest --output-on-failure

# 
# utils
# 

.PHONY: fmt
fmt:
	uvx --from cmakelang cmake-format --dangle-parens --line-width 120 -i CMakeLists.txt
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i

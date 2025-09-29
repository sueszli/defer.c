.PHONY: demo
demo:
	tmpdir=$$(mktemp -d) && gcc -o $$tmpdir/demo demo.c && $$tmpdir/demo && rm -rf $$tmpdir






DOCKER_RUN = docker run --rm -v $(PWD):/workspace main sh -c

.PHONY: build-image
build-image:
	docker build -t main .

# figure out how to run both in clang and gcc

.PHONY: run
run:
	$(DOCKER_RUN) "cd $(mktemp -d) && cmake /workspace && make -j$(nproc) && ./autograd"

.PHONY: fmt
fmt:
	uvx --from cmakelang cmake-format --dangle-parens --line-width 120 -i CMakeLists.txt
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i

.PHONY: clean
clean:
	docker rmi main

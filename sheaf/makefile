DOCKER_RUN = docker run --rm -v $(PWD):/workspace sheaf sh -c

.PHONY: build-image # build the docker image
build-image:
	docker build -t sheaf .

.PHONY: run # run the main program
run: build-image
	$(DOCKER_RUN) 'scons run && scons --clean -s'

.PHONY: demo-go # run the go demo program
demo-go: build-image
	$(DOCKER_RUN) 'scons run_demo_go && scons --clean -s'

.PHONY: demo-async # run the async demo program
demo-async: build-image
	$(DOCKER_RUN) 'scons run_demo_async && scons --clean -s'

.PHONY: test # run all tests
test: build-image
	$(DOCKER_RUN) 'scons test && scons --clean -s'

.PHONY: valgrind # run the main program under valgrind
valgrind: build-image
	$(DOCKER_RUN) 'scons valgrind && scons --clean -s'

.PHONY: fmt # format all source files
fmt:
	$(DOCKER_RUN) 'find . -name "*.c" -o -name "*.h" | xargs clang-format -i'

.PHONY: clean # remove all build artifacts
clean:
	docker rmi sheaf 2>/dev/null || true
	# docker system prune -a -f

.PHONY: help # generate help message
help:
	@echo "Usage: make [target]\n"
	@grep '^.PHONY: .* #' makefile | sed 's/\.PHONY: \(.*\) # \(.*\)/\1\t\2/' | expand -t20

PROJECT_ROOT := $(shell pwd)
export BIN_DIR=${PROJECT_ROOT}/bin
export SRC_DIR=${PROJECT_ROOT}/src
export OBJS_DIR=${PROJECT_ROOT}/objs

all: setec_astronomy tests

setec_astronomy: 
	${MAKE} -C src

tests: 
	${MAKE} -C tests

clean:
	rm objs/*.o bin/*

.PHONY : clean setec_astronomy all tests
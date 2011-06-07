PROJECT_ROOT=../..
BIN_DIR=${PROJECT_ROOT}/bin
SRC_DIR=${PROJECT_ROOT}/src
OBJS_DIR=${PROJECT_ROOT}/objs
CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/regen_test_data.o ${OBJS_DIR}/header.o ${OBJS_DIR}/util.o
LIBS=-lssl

${BIN_DIR}/test_regen_test_data: ${OBJS}
	${CC} ${OPTS} ${LIBS} -o $@ ${OBJS}

${OBJS_DIR}/regen_test_data.o: regen_test_data.c
	${CC} ${OPTS} -c regen_test_data.c -o $@

${OBJS_DIR}/header.o: ${SRC_DIR}/header.c ${SRC_DIR}/header.h
	${CC} ${OPTS} -c ${SRC_DIR}/header.c -o $@

${OBJS_DIR}/util.o: ${SRC_DIR}/util.c ${SRC_DIR}/util.h
	${CC} ${OPTS} -c ${SRC_DIR}/util.c -o $@

clean:
	rm ${OBJS} ${BIN_DIR}/regen_test_data

.PHONY: clean
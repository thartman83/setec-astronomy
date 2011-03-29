PROJECT_ROOT=../..
BIN_DIR=${PROJECT_ROOT}/bin
SRC_DIR=${PROJECT_ROOT}/src
OBJS_DIR=${PROJECT_ROOT}/objs
CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/test_key.o ${OBJS_DIR}/key.o
LIBS=-lmhash

${BIN_DIR}/test_key: ${OBJS}
	${CC} ${LIBS} ${OPTS} -o $@ ${OBJS}

${OBJS_DIR}/test_key.o: test_key.c test_key.h ${SRC_DIR}/key.h
	${CC} ${OPTS} -c test_key.c -o $@

${OBJS_DIR}/key.o: ${SRC_DIR}/key.c ${SRC_DIR}/key.h
	${CC} ${OPTS} -c ${SRC_DIR}/key.c -o $@

clean:
	rm ${OBJS} ${BIN_DIR}/test_key

.PHONY: clean
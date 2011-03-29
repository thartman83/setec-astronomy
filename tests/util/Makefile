PROJECT_ROOT=../..
BIN_DIR=${PROJECT_ROOT}/bin
SRC_DIR=${PROJECT_ROOT}/src
OBJS_DIR=${PROJECT_ROOT}/objs
CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/test_util.o ${OBJS_DIR}/util.o

${BIN_DIR}/test_util: ${OBJS}
	${CC} ${OPTS} -o $@ ${OBJS}

${OBJS_DIR}/test_util.o: test_util.c test_util.h ${SRC_DIR}/util.h
	${CC} ${OPTS} -c test_util.c -o $@

${OBJS_DIR}/util.o: ${SRC_DIR}/util.c ${SRC_DIR}/util.h
	${CC} ${OPTS} -c ${SRC_DIR}/util.c -o $@

clean:
	rm ${OBJS} ${BIN_DIR}/test_util
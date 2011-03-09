CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/test_util.o ${OBJS_DIR}/util.o

ifeq (0,${MAKELEVEL})

all:
	${MAKE} tests -C ../../

else

${BIN_DIR}/test_util: ${OBJS}
	${CC} ${OPTS} -o $@ ${OBJS}

${OBJS_DIR}/test_util.o: test_util.c test_util.h ${SRC_DIR}/util.h
	${CC} ${OPTS} -c test_util.c -o $@

${OBJS_DIR}/util.o: ${SRC_DIR}/util.c ${SRC_DIR}/util.h
	${CC} ${OPTS} -c ${SRC_DIR}/util.c -o $@

endif
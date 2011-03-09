CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/test_header.o ${OBJS_DIR}/header.o ${OBJS_DIR}/util.o

ifeq (0,${MAKELEVEL})

all:
	${MAKE} tests -C ../../

else

${BIN_DIR}/test_header: ${OBJS}
	${CC} ${OPTS} -o $@ ${OBJS}

${OBJS_DIR}/test_header.o: test_header.h test_header.c ${SRC_DIR}/header.h
	${CC} ${OPTS} -c test_header.c -o $@

${OBJS_DIR}/header.o: ${SRC_DIR}/header.h ${SRC_DIR}/header.c ${SRC_DIR}/errors.h \
											${SRC_DIR}/util.h
	${CC} ${OPTS} -c ${SRC_DIR}/header.c -o $@

${OBJS_DIR}/util.o: ${SRC_DIR}/util.h ${SRC_DIR}/util.c
	${CC} ${OPTS} -c ${SRC_DIR}/util.c -o $@

endif
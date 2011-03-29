PROJECT_ROOT=../..
BIN_DIR=${PROJECT_ROOT}/bin
SRC_DIR=${PROJECT_ROOT}/src
OBJS_DIR=${PROJECT_ROOT}/objs
CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/little_black_box.o ${OBJS_DIR}/test_little_black_box.o ${OBJS_DIR}/header.o \
		 ${OBJS_DIR}/util.o ${OBJS_DIR}/key.o
LIBS=-lmcrypt -lcrypto -lmhash

${BIN_DIR}/test_little_black_box: ${OBJS}
	${CC} ${OTPS} ${LIBS} -o ${BIN_DIR}/test_little_black_box ${OBJS}

${OBJS_DIR}/little_black_box.o: ${SRC_DIR}/little_black_box.h ${SRC_DIR}/little_black_box.c \
	  													 ${SRC_DIR}/header.h ${SRC_DIR}/key.h ${SRC_DIR}/errors.h
	${CC} ${OPTS} -c ${SRC_DIR}/little_black_box.c -o $@

${OBJS_DIR}/test_little_black_box.o: test_little_black_box.c test_little_black_box.h \
                                     ${SRC_DIR}/little_black_box.h ${SRC_DIR}/little_black_box.c
	${CC} ${OPTS} -c test_little_black_box.c -o $@

${OBJS_DIR}/header.o: ${SRC_DIR}/header.h ${SRC_DIR}/header.c
	${CC} ${OPTS} -c ${SRC_DIR}/header.c -o $@

${OBJS_DIR}/util.o: ${SRC_DIR}/util.h ${SRC_DIR}/util.c
	${CC} ${OPTS} -c ${SRC_DIR}/util.c -o $@

${OBJS_DIR}/key.o: ${SRC_DIR}/key.h ${SRC_DIR}/key.c
	${CC} ${OPTS} -c ${SRC_DIR}/key.c -o $@

clean:
	rm ${OBJS} ${BIN_DIR}/test_little_black_box
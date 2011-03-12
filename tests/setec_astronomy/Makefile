CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/little_black_box.o ${OBJS_DIR}/test_setec_astronomy.o ${OBJS_DIR}/header.o \
		 ${OBJS_DIR}/util.o ${OBJS_DIR}/key.o ${OBJS_DIR}/setec_astronomy.o
LIBS=-lmcrypt -lcrypto

ifeq (0,${MAKELEVEL})

all:
	${MAKE} tests -C ../../

else

${BIN_DIR}/test_setec_astronomy: ${OBJS}
	${CC} ${OPTS} ${LIBS} -o ${BIN_DIR}/test_setec_astronomy ${OBJS}

${OBJS_DIR}/test_setec_astronomy.o: ${SRC_DIR}/setec_astronomy.c ${SRC_DIR}/setec_astronomy.h \
																		test_setec_astronomy.h test_setec_astronomy.c
	${CC} ${OPTS} ${LIBS} -c test_setec_astronomy.c -o $@

${OBJS_DIR}/setec_astronomy.o: ${SRC_DIR}/setec_astronomy.c ${SRC_DIR}/setec_astronomy.h \
															 ${SRC_DIR}/little_black_box.h
	${CC} ${OPTS} ${LIBS} -c ${SRC_DIR}/setec_astronomy.c -o $@

${OBJS_DIR}/little_black_box.o: ${SRC_DIR}/little_black_box.h ${SRC_DIR}/little_black_box.c \
	  													 ${SRC_DIR}/header.h ${SRC_DIR}/key.h ${SRC_DIR}/errors.h
	${CC} ${OPTS} -c ${SRC_DIR}/little_black_box.c -o $@

${OBJS_DIR}/header.o: ${SRC_DIR}/header.h ${SRC_DIR}/header.c
	${CC} ${OPTS} -c ${SRC_DIR}/header.c -o $@

${OBJS_DIR}/util.o: ${SRC_DIR}/util.h ${SRC_DIR}/util.c
	${CC} ${OPTS} -c ${SRC_DIR}/util.c -o $@

${OBJS_DIR}/key.o: ${SRC_DIR}/key.h ${SRC_DIR}/key.c
	${CC} ${OPTS} -c ${SRC_DIR}/key.c -o $@

endif
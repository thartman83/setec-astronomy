CC=gcc
OPTS=-Wall -g
OBJS=${OBJS_DIR}/little_black_box.o ${OBJS_DIR}/util.o 
OBJS+=${OBJS_DIR}/setec_astronomy.o ${OBJS_DIR}/header.o ${OBJS_DIR}/main.o
OBJS+=${OBJS_DIR}/errors.o
LIBS=-lmcrypt -lssl

ifeq (0, ${MAKELEVEL})
# can't be called by itself, gets export info from ../Makefile
all:
	${MAKE} setec_astronomy -C ../
else

setec_astronomy: ${OBJS}
	${CC} ${OPTS} ${LIBS} -o ${BIN_DIR}/$@ ${OBJS}

${OBJS_DIR}/main.o: main.c setec_astronomy.h
	${CC} ${OPTS} ${LIBS} -c main.c -o $@

${OBJS_DIR}/setec_astronomy.o: setec_astronomy.c little_black_box.h errors.h
	${CC} ${OPTS} -c setec_astronomy.c -o $@

${OBJS_DIR}/little_black_box.o: little_black_box.c little_black_box.h header.h errors.h
	${CC} ${OPTS} -c little_black_box.c -o $@

${OBJS_DIR}/util.o: util.h util.c
	${CC} ${OPTS} -c util.c -o $@

${OBJS_DIR}/header.o: header.h header.c util.h errors.h
	${CC} ${OPTS} -c header.c -o $@

${OBJS_DIR}/errors.o: errors.h errors.c
	${CC} ${OPTS} -c errors.c -o $@

.PHONY : setec_astronomy

endif
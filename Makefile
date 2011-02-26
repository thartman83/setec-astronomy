CXX=gcc
OPTS=-Wall -g
OBJS_DIR=objs
SRC_DIR=src
TEST_DIR=tests
BIN_DIR=bin
OBJS=${OBJS_DIR}/setecAstronomy.o ${OBJS_DIR}/header.o ${OBJS_DIR}/util.o
TEST_OBJS=${OBJS_DIR}/test_header.o ${OBJS_DIR}/test_util.o ${OBJS_DIR}/test_setecAstronomy.o

all: setecAstronomy tests

tests: test_setecAstronomy

setecAstronomy: ${OBJS}
	${CXX} ${OPTS} -o ${BIN_DIR}/setecAstronomy ${OBJS} ${OBJS_DIR}/main.o

${OBJS_DIR}/main.o: ${SRC_DIR}/setecAstronomy.h ${SRC_DIR}/main.c
	${CXX} ${OPTS} -c ${SRC_DIR}/main.c -o ${OBJS_DIR}/main.o

${OBJS_DIR}/setecAstronomy.o: ${SRC_DIR}/setecAstronomy.c ${SRC_DIR}/setecAstronomy.h
	${CXX} ${OPTS} -c ${SRC_DIR}/setecAstronomy.c -o ${OBJS_DIR}/setecAstronomy.o

${OBJS_DIR}/header.o: ${SRC_DIR}/header.h ${SRC_DIR}/errors.h ${SRC_DIR}/util.h ${SRC_DIR}/header.c
	${CXX} ${OPTS} -c ${SRC_DIR}/header.c -o ${OBJS_DIR}/header.o

${OBJS_DIR}/util.o: ${SRC_DIR}/util.h ${SRC_DIR}/util.c
	${CXX} ${OPTS} -c ${SRC_DIR}/util.c -o ${OBJS_DIR}/util.o

test_setecAstronomy: ${TEST_OBJS} ${OBJS}
	${CXX} ${OPTS} -o ${BIN_DIR}/test_setecAstronomy ${TEST_OBJS} ${OBJS}

${OBJS_DIR}/test_setecAstronomy.o: ${TEST_DIR}/test_util.h ${TEST_DIR}/test_header.h ${TEST_DIR}/test_setecAstronomy.c
	${CXX} ${OPTS} -c ${TEST_DIR}/test_setecAstronomy.c -o ${OBJS_DIR}/test_setecAstronomy.o

${OBJS_DIR}/test_util.o: ${TEST_DIR}/test_util.h ${SRC_DIR}/util.h ${SRC_DIR}/util.c ${TEST_DIR}/test_util.c
	${CXX} ${OPTS} -c ${TEST_DIR}/test_util.c -o ${OBJS_DIR}/test_util.o

${OBJS_DIR}/test_header.o: ${SRC_DIR}/header.h ${SRC_DIR}/header.c ${TEST_DIR}/test_header.h ${TEST_DIR}/test_header.c ${TEST_DIR}/unit_test.h
	${CXX} ${OPTS} -c ${TEST_DIR}/test_header.c -o ${OBJS_DIR}/test_header.o

clean:
	rm ${OBJS_DIR}/*.o ${BIN_DIR}/*

.PHONY : clean setecAstronomy test_setecAstronomy all tests
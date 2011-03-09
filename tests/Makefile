TESTS=header little_black_box key util setec_astronomy

ifeq (0, ${MAKELEVEL})
# can't be called by itself, gets export info from ../Makefile
all:
	${MAKE} tests -C ../
else
all: $(TESTS)

$(TESTS):
	${MAKE} -C $@

.PHONY : ${TESTS}
endif
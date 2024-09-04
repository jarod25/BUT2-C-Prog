CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -O

ARCHIVE = TP5.tar.gz

SRC =	1_execute_haut.c \
	1_execute_bas.c \
	2_sigaction_bsd.c \
	2_sigaction_sysv.c \
	2_signal_bsd.c \
	2_signal_sysv.c \
	3_synchro.c

BIN = $(SRC:%.c=%)

.PHONY: all clean tar

all: $(BIN)

tar: $(ARCHIVE)

clean:
	$(RM) $(BIN) $(ARCHIVE)

$(ARCHIVE): Makefile $(SRC)
	$(MAKE)
	tar zcf $@ $^

-include Makefile.siggen

CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -O

ARCHIVE = TP4.tar.gz

SRC =	1_fork_simple.c \
	2_petits-fils.c \
	3_creerterm.c \
	4_creerterm_root.c \
	5_system.c \
	verif_system.c

BIN = $(SRC:%.c=%)

.PHONY: all clean tar

all: $(BIN)

tar: $(ARCHIVE)

clean:
	$(RM) $(BIN) $(ARCHIVE)

$(ARCHIVE): Makefile $(SRC)
	$(MAKE)
	tar zcf $@ $^

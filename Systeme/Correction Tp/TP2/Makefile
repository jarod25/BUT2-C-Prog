CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -O

ARCHIVE = TP2.tar.gz

SRC =	1_copie_block.c \
	1_copie_char.c \
	2_binhexasc.c \
	3_copie.c

BIN = $(SRC:%.c=%)

.PHONY: all clean tar

all: $(BIN)

tar: $(ARCHIVE)

clean:
	$(RM) $(BIN) $(ARCHIVE)

$(ARCHIVE): Makefile $(SRC)
	$(MAKE)
	tar zcf $@ $^

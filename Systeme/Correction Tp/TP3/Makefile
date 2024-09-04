CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -O

ARCHIVE = TP3.tar.gz

SRC =	1_cat.c \
	1_cat_multiple.c \
	2_tab.c \
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

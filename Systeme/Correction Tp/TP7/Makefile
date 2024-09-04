CC = gcc
CFLAGS = -std=gnu11 -g -Wall -Wextra -O2 -pthread
LDLIBS = -lm

ARCHIVE = TP6.tar.gz

SRC_C =	1_threads.c \
	2_calcul.c \
	3_concur.c

SRC_JAVA = Ex1_Threads.java \
	   Ex2_Calcul.java \
	   Ex3_Concur.java

SRC = $(SRC_C) $(SRC_JAVA)

BIN = $(SRC_C:%.c=%) $(SRC_JAVA:%.java=%.class)

JAVAC = javac
JAVAFLAGS = -Xlint

.PHONY: all clean tar

all: $(BIN)

%.class: %.java
	$(JAVAC) $(JAVAFLAGS) $<

tar: $(ARCHIVE)

clean:
	$(RM) $(BIN:%.class=%*.class) $(ARCHIVE)

$(ARCHIVE): Makefile $(SRC)
	$(MAKE)
	tar zcf $@ $^

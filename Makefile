OBJS=main.o tlr.o
CFLAGS=-g -I. -Wall -Wextra
BIN=tlrtest
CC=gcc

%.o:%.c
	$(CC) $(CFLAGS) $(DEFINES) -o $@ -c $<

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(DEFINES) -o $(BIN) $^

clean:
	rm $(BIN) $(OBJS)

CC=gcc
CFLAGS=-Wall -Wextra -O2
SRC=src
days=$(basename $(notdir $(wildcard $(SRC)/day*.c)))

all: $(days)

$(days):
	$(CC) $(CFLAGS) $(SRC)/$@.c $(SRC)/input_helper.c -o $@.exe

clean:
	del /Q *.exe
	del /Q outputs\*

.PHONY: all clean $(days)

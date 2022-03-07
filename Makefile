CC=g++
CFLAGS=-Wall
SRC=rbn_example.cpp
EXE=rbn_example

default: run

.PHONY: run
run: $(EXE)
	./$^

$(EXE): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^


.PHONY: clean
clean:
	rm -f $(EXE)
	rm -f run.log
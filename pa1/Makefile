SRCS = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)
CC = gcc
CFLAGS = -g -std=c99
LDFLAGS = -lm

default: simulator

simulator: $(SRCS) $(HEADERS)
	@echo "Building $@..."
	@#gcc simulator.o -o simulator -ggdb -std=c99
	@echo "Sources: $(SRCS)"
	@echo "Headers: $(HEADERS)"
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

clean:
	-rm -f simulator 
	-rm -f *_trace.txt *.out mdump.txt cdump.txt pa*out.txt

clean_all:
	-rm -f simulator 
	-rm -rd simulator.dSYM
	-rm -f *_trace.txt *.out mdump.txt cdump.txt pa*out.txt
	-rm -rf assembled_tests

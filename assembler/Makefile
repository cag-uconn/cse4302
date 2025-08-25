HEADERS = file_parser.h

default: assembler

assembler: assembler.c file_parser.c
	gcc assembler.c file_parser.c -o assembler -std=c99

clean:
	-rm -f assembler.o
	-rm -f assembler

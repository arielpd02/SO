
#PHONY indica que no son archivos.
.PHONY: all clean
BIN= main
OBJ = ${CSRC:.c=.o}

all: main
clean: 
	rm -f $(BIN) $(OBJ)

main:	agregarInfoUser.c cuit_verifier.o
	gcc -o $@ $^

%.o: %.c
	gcc -c -o $@  $<
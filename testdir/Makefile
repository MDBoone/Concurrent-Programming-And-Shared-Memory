CC = gcc
OUT_EXE = Starter
FILES = Starter.c Prime.c Total.c Fibb.c CharacterReader.c

build:
	$(CC) -o Starter Starter.c -I. -Wall -Wfatal-errors -lrt
	$(CC) -o Prime Prime.c -I. -Wall -Wfatal-errors -lrt
	$(CC) -o Total Total.c -I. -Wall -Wfatal-errors -lrt
	$(CC) -o Fibb Fibb.c -I. -Wall -Wfatal-errors -lrt
	$(CC) -o CharacterReader CharacterReader.c -I -Wall -Wfatal-errors -lrt

clean:
	rm -f $(OUT_EXE)
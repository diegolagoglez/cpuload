
CC := g++
EXE := cpuload
SRC := cpuload.cpp

all:
	$(CC) -o $(EXE) $(SRC)

.PHONY: clean

clean:
	rm *.o $(EXE)

run: all
	./$(EXE)

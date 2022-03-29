sgraph: main.o graf.o
	$(CC) -o sgraph $^

io_test: io_test.o graf.o
	$(CC) -o iotest $^

main.o: src/main.c
graf.o: src/graf.c src/graf.h
io_test.o: tests/io_test.c

.PHONY: clean

clean:
	-rm *.o
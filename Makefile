sgraph:
	$(CC) -o sgraph src/*.c

io_test:
	$(CC) -o iotest src/graf.c tests/io_test.c

.PHONY: clean

clean:
	-rm *.o
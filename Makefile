sgraph:
	$(CC) -o sgraph src/main.c src/graf.c src/queue.c

io_test:
	$(CC) -o iotest src/graf.c src/io_test.c src/queue.c

.PHONY: clean

clean:
	-rm *.o
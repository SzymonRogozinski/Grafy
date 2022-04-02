sgraph:
	$(CC) -o sgraph src/main.c src/graf.c src/queue.c src/prqueue.c

io_test:
	$(CC) -o io_test src/graf.c src/io_test.c src/queue.c src/prqueue.c

.PHONY: clean

clean:
	-rm *.o
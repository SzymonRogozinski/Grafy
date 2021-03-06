sgraph:
	$(CC) -o sgraph src/main.c src/graf.c src/queue.c src/prqueue.c

io_test:
	$(CC) -o io_test src/graf.c src/io_test.c src/queue.c src/prqueue.c

gen_test:
	$(CC) -o gen_test src/graf.c src/gen_test.c src/queue.c src/prqueue.c

q_test:
	$(CC) -o q_test src/queue.c src/q_test.c

.PHONY: clean

clean:
	-rm *.o
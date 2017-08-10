query:query.c
	gcc -o query query.c -D_GNU_SOURCE -D_USE_XOPEN
clean:
	rm -rf query.o query

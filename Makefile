bin/client: src/1.c
	gcc src/1.c -o 1 -fsanitize=leak,address

clean:
	rm src/1.c
	rmdir src

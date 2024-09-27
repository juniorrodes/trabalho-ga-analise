build:
	gcc -g kadane.c -o kadane -O3

run: build
	./kadane

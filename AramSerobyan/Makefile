all:
	cc -Wall -c execute.c input_handler.c
	ar -cvq libctest.a execute.o input_handler.o
	cc -o main main.c libctest.a 
	./main
	rm libctest.a

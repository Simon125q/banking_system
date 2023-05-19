all: bank

bank: bank.o
	gcc -g $^ -fsanitize=undefined -o bank
.c.o: 
	gcc -fsanitize=undefined -g -Wall -pedantic -c  $<

bank.o: bank.c

clean:
	-rm bank.o bank
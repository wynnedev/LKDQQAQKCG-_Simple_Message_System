all: master slave
clean:
	rm master slave

master: master.c
	gcc -o master master.c -I.

slave: slave.c
	gcc -o slave slave.c -I.
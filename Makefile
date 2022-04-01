master: master.c
	gcc -O master master.c -I

slave: slave.c
	gcc -O slave slave.c -I
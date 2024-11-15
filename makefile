a2:
	gcc -Wall -Wpedantic -march=haswell -O3 -c list.c a2.c socket_thread.c send_receive.c && gcc list.o a2.o send_receive.o socket_thread.o -o a2 -pthread
clean:
	rm *.o a2

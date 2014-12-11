nmacs: buffer.o menu.o main.o  
	gcc -g  buffer.o   -o nmacs -lncurses

#main.o: main.cpp
#	g++ -g -c main.cpp -o main.o

buffer.o: buffer.c
	gcc -g -c `pkg-config --cflags --libs glib-2.0`  buffer.c -o buffer.o

#menu.o: menu.cpp
#	g++ -g -c menu.cpp -o menu.o


clean: 
	rm  buffer.o #menu.o main.o main

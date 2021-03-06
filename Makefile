nmacs: buffer.o menu.o main.o  
	g++ -g main.o buffer.o menu.o  -o nmacs -lncurses -lboost_system -lboost_filesystem

main.o: main.cpp
	g++ -g -c main.cpp -o main.o

buffer.o: buffer.cpp
	g++ -g -c buffer.cpp -o buffer.o

menu.o: menu.cpp
	g++ -g -c menu.cpp -o menu.o

clean: 
	rm  buffer.o menu.o main.o nmacs

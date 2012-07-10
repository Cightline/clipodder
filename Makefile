CC=g++ 

CONFIG =`pkg-config --cflags --libs libconfuse` 
CURL   =`pkg-config --cflags --libs libcurl`
XML    =`pkg-config --cflags --libs libxml-2.0`


.phony = *.o clipodder

network: network.cpp network.hpp
	$(CC) network.cpp -c -o network.o $(CURL)


format: format.cpp format.hpp
	$(CC) format.cpp -c -o format.o 


parser: parser.cpp
	$(CC) parser.cpp -c -o parser.o $(XML) 


config_parser: config_parser.cpp config_parser.hpp
	$(CC) config_parser.cpp -c -o config_parser.o -Wno-write-strings $(CONFIG)


filesystem: filesystem.cpp filesystem.hpp
	$(CC) filesystem.cpp -c -o filesystem.o 


file_manager: file_manager.cpp file_manager.hpp
	$(CC) file_manager.cpp -c -o file_manager.o


utils: utils.cpp utils.hpp
	$(CC) utils.cpp -c -o utils.o


core: core.cpp core.hpp
	$(CC) core.cpp -c -o core.o 


main: main.cpp main.hpp container.hpp
	$(CC) main.cpp -c -o main.o $(CONFIG) $(XML)


all:  network format parser core filesystem file_manager config_parser main utils
	$(CC) \
	main.o network.o format.o parser.o config_parser.o filesystem.o file_manager.o core.o utils.o \
	-o clipodder $(CONFIG) $(XML) $(CURL)





install: all
	mv clipodder /usr/bin/clipodder
	chmod +x /usr/bin/clipodder
	rm *.o

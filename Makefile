CC=g++ -g

LIBS=`pkg-config --cflags --libs libconfuse` \
     `pkg-config --cflags --libs libcurl` \
     `pkg-config --cflags --libs libxml-2.0`



network: network.cpp network.hpp
	$(CC) network.cpp -c -o network.o $(LIBS)


format: format.cpp format.hpp
	$(CC) format.cpp -c -o format.o 


parser: parser.cpp parser.hpp
	$(CC) parser.cpp -c -o parser.o $(LIBS)


config: config.cpp config.hpp
	$(CC) config.cpp -c -o config.o $(LIBS)


core: core.cpp core.hpp
	$(CC) core.cpp -c -o core.o $(LIBS)

main: main.cpp main.hpp
	$(CC) main.cpp -c -o main.o $(LIBS)


all: network format parser core config main
	$(CC) main.o network.o format.o parser.o config.o core.o -o clipodder $(LIBS)

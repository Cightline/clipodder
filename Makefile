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


clipodder: clipodder.cpp clipodder.hpp
	$(CC) clipodder.cpp -c -o clipodder.o $(LIBS)


all: network format parser clipodder config
	$(CC) network.o format.o parser.o config.o clipodder.o -o clipodder $(LIBS)

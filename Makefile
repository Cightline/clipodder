CC=g++ -g

CONFIG =`pkg-config --cflags --libs libconfuse` 
CURL   =`pkg-config --cflags --libs libcurl`
XML    =`pkg-config --cflags --libs libxml-2.0`


.phony = *.o clipodder

network: network.cpp network.hpp
	$(CC) network.cpp -c -o network.o $(CURL)


format: format.cpp format.hpp
	$(CC) format.cpp -c -o format.o 


parser: parser.cpp parser.hpp
	$(CC) parser.cpp -c -o parser.o $(XML) $(CONFIG)


config: config.cpp config.hpp
	$(CC) config.cpp -c -o config.o $(CONFIG)


filesystem: filesystem.cpp filesystem.hpp
	$(CC) filesystem.cpp -c -o filesystem.o 


downloader: downloader.cpp downloader.hpp
	$(CC) downloader.cpp -c -o downloader.o $(CURL)


core: core.cpp core.hpp 
	$(CC) core.cpp -c -o core.o $(CURL) $(XML) $(CONFIG)


main: main.cpp main.hpp
	$(CC) main.cpp -c -o main.o $(CONFIG) $(XML)



all:  network format parser core filesystem downloader config main
	$(CC) main.o network.o format.o parser.o config.o filesystem.o \
	      downloader.o core.o debug.hpp -o clipodder $(CONFIG) $(XML) $(CURL)

	rm *.o



install: all
	cp clipodder /usr/bin/clipodder
	chmod +x /usr/bin/clipodder

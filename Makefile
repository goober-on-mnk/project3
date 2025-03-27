# Simple build for the AjaxDemoclientserver to lookup Bible thing

USER= dergill

CC= g++
CFLAGS= -g -std=c++11

all:	client server  PutCGI PutHTML

client.o: 	client.cpp fifo.h Ref.h Verse.h Bible.h
		$(CC) $(CFLAGS) -c client.cpp

server.o:	server.cpp fifo.h Ref.h Verse.h Bible.h
		$(CC) $(CFLAGS) -c server.cpp

client:		client.o fifo.o Ref.o Verse.o Bible.o
		$(CC) $(CFLAGS) -o client client.o fifo.o -L/usr/local/lib -lcgicc

server: 	server.o fifo.o Ref.o Verse.o Bible.o
		$(CC) $(CFLAGS) -o server server.o fifo.o Ref.o Verse.o Bible.o

fifo.o:		fifo.cpp fifo.h
		$(CC) $(CFLAGS) -c fifo.cpp

Ref.o : 	Ref.h Ref.cpp
		$(CC) $(CFLAGS) -c Ref.cpp

Verse.o : 	Ref.h Verse.h Verse.cpp
		$(CC) $(CFLAGS) -c Verse.cpp

Bible.o : 	Ref.h Verse.h Bible.h Bible.cpp
		$(CC) $(CFLAGS) -c Bible.cpp


PutCGI: client
		chmod 757 client
		cp client /var/www/html/class/csc3004/dergill/cgi-bin
		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/dergill/cgi-bin/

PutHTML: bibleindex.html
		cp bibleindex.html  /var/www/html/class/csc3004/dergill
		echo "Current contents of your HTML directory: "
		ls -l  /var/www/html/class/csc3004/dergill

clean:
	rm *.o *# *~ client server



CC = gcc

LIBDIR =  -L../lib
LIB = -lterm -lpthread -llog

INCLDIR = -I../incl

CFLAGS = -g -Wall

OBJS = main.o mainwin.o tftp.o yplayer.o

all: nplay

nplay: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBDIR) $(LIB) 
	mv $@  ../bin

yplay: yplayer.o
	$(CC) -o $@ $^ $(LIBDIR) $(LIB) 

flist: tftp.o flist.o
	$(CC) -o $@ $^ $(LIBDIR) $(LIB) 

%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLDIR) $<
	
clean:
	rm -f *.o

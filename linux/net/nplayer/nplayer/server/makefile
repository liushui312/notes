
CC = gcc

LIBDIR = -L../lib
LIB = -l pthread -llog

INCLDIR = -I../incl

CFLAGS = -g -Wall

OBJS = main.o dowork.o

all: nplayd


nplayd: tftpd.o
	$(CC) -o $@ $^ $(LIBDIR) $(LIB)
	mv $@ ../bin
	
%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLDIR) $<
clean:
	rm -f *.o


CC	= @gcc 
CFLAGS  = -O2 -Wall -g

SOURCES	= main.c jpeg.c bmp.c fb_display.c wav/wav.c

OBJECTS	= ${SOURCES:.c=.o}

OUT	= viewer
LIBS = -ljpeg -lpthread

all: $(OUT)
	@echo Build Done.

$(OUT): $(OBJECTS)
	@$(CC) $(LDFLAGS) -o $(OUT) $(OBJECTS) $(LIBS)

clean:
	@rm -f $(OBJECTS) $(OUT)
	@echo Remove Done.


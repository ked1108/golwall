CC = gcc

CFLAGS = -pedantic -Wall -Wextra 

LFLAGS = `sdl2-config --cflags --libs` -lX11 -fopenmp

SRCS = main.c 

OUT = golwall-x

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(OUT) $(SRCS) 

clean:
	rm -f $(OUT)

install:
	install -m 755 $(OUT) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(OUT)

CC = gcc

CFLAGS = -pedantic -Wall -Wextra 

SRCS = main.c lodepng.c

OUT = golwall-sway

all: $(OUT)

$(OUT): $(SRCS)
	$(CC) $(CFLAGS) -o $(OUT) $(SRCS) 

clean:
	rm -f $(OUT)

install:
	install -m 755 $(OUT) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(OUT)

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pedantic -Wall -Wextra 

# Source files
SRCS = main.c lodepng.c

# Output binary
OUT = dynamic_wallpaper

# Default rule
all: $(OUT)

# Compile the program
$(OUT): $(SRCS)
	$(CC) $(CFLAGS) -o $(OUT) $(SRCS) 

# Clean the build
clean:
	rm -f $(OUT)

# Install the program (optional)
install:
	install -m 755 $(OUT) /usr/local/bin/

# Uninstall the program (optional)
uninstall:
	rm -f /usr/local/bin/$(OUT)

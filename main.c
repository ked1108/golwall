#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "lodepng.h"

#define ROWS 36
#define COLS 64

#define IMG_WIDTH   1920
#define IMG_HEIGHT  1080


int grid[ROWS][COLS] = {0};
int buff[ROWS][COLS] = {0};


// ONE STEP ENCODING TO FILE
void encodeOneStep(const char* filename, const unsigned char* image) {
  unsigned error = lodepng_encode32_file(filename, image, IMG_WIDTH, IMG_HEIGHT);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}

void convert_to_image(unsigned char* image) {
    float x_ratio = (float)IMG_WIDTH   / COLS;
    float y_ratio = (float)IMG_HEIGHT  / ROWS;

    for(int y = 0; y < IMG_HEIGHT; y++) { 
        for(int x = 0; x < IMG_WIDTH; x++) {
            int src_x = (int)x / x_ratio;
            int src_y = (int)y / y_ratio;

            if (src_y >= ROWS) src_y = ROWS - 1;
            if (src_x >= COLS) src_x = COLS - 1;
            
            int dst = 4 * y * IMG_WIDTH + 4 * x;
            image[dst+0] = grid[src_y][src_x] ? 255 : 0;
            image[dst+1] = grid[src_y][src_x] ? 255 : 0;
            image[dst+2] = grid[src_y][src_x] ? 255 : 0;
            image[dst+3] = 255;
        }
    }

}


void display() {
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            if (grid[y][x]) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

int mod(int a, int b) {
    return (a%b + b)%b;
}

int count_neighbours(int cx, int cy) {
    int neighbours = 0;
    for(int dx = -1; dx <= 1; dx++) {
        for(int dy = -1; dy <= 1; dy++) {
            int x = mod(cx+dx, COLS);
            int y = mod(cy+dy, ROWS);
            if(grid[y][x]) neighbours++;
        }
    }

    return neighbours;
}

void step() {
    for(int y = 0; y < ROWS; y++) {
        for(int x = 0; x < COLS; x++) {
            int neighbours = count_neighbours(x, y);
            buff[y][x] = grid[y][x] ? (neighbours == 2 || neighbours == 3) : neighbours == 3;
        }
    }
}

void generate(){
    int fd = open("/dev/urandom", O_RDONLY);
    
    for(int y = 0; y < ROWS; y++) {
        for(int x = 0; x < COLS; x++) {
            int num;
            read(fd, &num, sizeof(num));
            grid[y][x] = num%2;
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        printf("Usage: %s <path to wallpaper>\n", argv[0]);
        return 1;
    }
    char* filepath = argv[1];
    generate();

    unsigned char* image = malloc(IMG_WIDTH * IMG_HEIGHT * 4);
    char* command = "nitrogen --set-zoom-fill";

    asprintf(&command, "nitrogen --set-zoom-fill %s", filepath);
    convert_to_image(image);
    encodeOneStep(filepath, image);
    system(command);

    int iter = 0;
    while (1) {
        convert_to_image(image);
        encodeOneStep(filepath, image);
        step();
        memcpy(grid, buff, sizeof(grid));
        usleep(5000*1000);
        iter++;
        if(iter == 10) generate();
    }

    return 0;
}

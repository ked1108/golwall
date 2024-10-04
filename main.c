#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>
#include <X11/Xlib.h>


#define ROWS (10*4)
#define COLS (16*4)

#define CELL_SIZE 30

typedef struct {
	Display* disp;
	SDL_Window* window;
	SDL_Renderer* renderer;
} Video;


int grid[ROWS][COLS] = {0};
int buff[ROWS][COLS] = {0};

/********************* SDL ************************/

static Video Setup(void) {
    Video self;
    self.disp = XOpenDisplay(NULL);
	if(!self.disp) {
		fprintf(stderr, "cant open display :(");
	}
    const Window x11w = RootWindow(self.disp, DefaultScreen(self.disp));
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "cant intialise SDL :( :%s", SDL_GetError());
	};
    self.window = SDL_CreateWindowFrom((void*) x11w);
	if(!self.window) {
		fprintf(stderr, "cant create window :( :%s", SDL_GetError());
	}
    self.renderer = SDL_CreateRenderer(self.window, 
									   -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return self;
}

static void Clear(Video* self) {
	SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255); 
	SDL_RenderClear(self->renderer);
}

static void Teardown(Video* self) {
    XCloseDisplay(self->disp);
    SDL_Quit();
    SDL_DestroyWindow(self->window);
    SDL_DestroyRenderer(self->renderer);
}

static void renderGrid(Video* self) {
	for(int row = 0; row < ROWS; row++) {
		for(int col = 0; col < COLS; col++) {
			if(grid[row][col] == 1) {
				SDL_Rect cell;
				cell.x = col * CELL_SIZE;
				cell.y = row * CELL_SIZE;
				cell.w = CELL_SIZE;
				cell.h = CELL_SIZE;
				SDL_SetRenderDrawColor(self->renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(self->renderer, &cell);
			}
		}
	}
}

/********************** GOL ***********************/

int mod(int a, int b) {
    return (a%b + b)%b;
}

int count_neighbours(int cx, int cy) {
    int neighbours = 0;
    for(int dx = -1; dx <= 1; dx++) {
        for(int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) continue;
            int x = mod(cx+dx, COLS);
            int y = mod(cy+dy, ROWS);
            if(grid[y][x]) neighbours++; }
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

int main() {
    generate();
    Video video = Setup();

    for(int i = 0; ;i++) {
		Clear(&video);
        renderGrid(&video);
        SDL_RenderPresent(video.renderer);
        step();
        memcpy(grid, buff, sizeof(grid));
		SDL_Delay(100);
        if(i%100 == 0) generate();

		SDL_Event event;
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT)
            break;
    }

    Teardown(&video);

    return 0;
}

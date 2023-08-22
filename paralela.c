// Juan Diego Avila Sagastume - 20090
// Diego Jose Franco -  20240
// Manuel Alejandro Archila - 161250

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct {
    float x, y;
    float dx, dy;
    SDL_Color color;
    int radio;
    bool relleno;
} Circle;

void moveCircle(Circle *circle) {
    circle->x += circle->dx;
    circle->y += circle->dy;

    if (circle->x <= 0 || circle->x >= WIDTH) circle->dx = -circle->dx;
    if (circle->y <= 0 || circle->y >= HEIGHT) circle->dy = -circle->dy;
}

void drawCircle(SDL_Renderer* renderer, int x_center, int y_center, int r) {
    
    for (int w = 0; w < r * 2; w++) {
        for (int h = 0; h < r * 2; h++) {
            int dx = r - w; 
            int dy = r - h; 
            if ((dx * dx + dy * dy) <= (r * r)) {
                SDL_RenderDrawPoint(renderer, x_center + dx, y_center + dy);
            }
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Se debe ingresar el numero de circulos");
        return 1;
    }

    int n = atoi(argv[1]);

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Circle circles[n];
    srand(time(NULL));

    #pragma omp parallel for num_threads(n)
    for (int i = 0; i < n; i++) {
        circles[i] = (Circle){
            rand() % WIDTH, rand() % HEIGHT,
            (rand() % 5 + 1) * (rand() % 2 ? 1 : -1),
            (rand() % 5 + 1) * (rand() % 2 ? 1 : -1),
            {rand() % 256, rand() % 256, rand() % 256, 255},
            rand() % 50 + 10
        };
    }

    bool running = true;
    Uint32 startTime;
    int frameCount = 0;

    while (running) {
        startTime = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        #pragma omp parallel for num_threads(n)
        for(int i = 0; i < n; i++) {
            moveCircle(&circles[i]);
        }

        for (int i = 0; i < n; i++) {
            SDL_SetRenderDrawColor(renderer, circles[i].color.r, circles[i].color.g, circles[i].color.b, circles[i].color.a);
            drawCircle(renderer, circles[i].x, circles[i].y, circles[i].radio);
        }

        

        SDL_RenderPresent(renderer);
        frameCount++;

        if (SDL_GetTicks() - startTime < 1000 / 30) {
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - startTime));
        }

        if (frameCount % 30 == 0) {
            printf("FPS: %f\n", 30.0 * 1000 / (SDL_GetTicks() - startTime + 1));
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


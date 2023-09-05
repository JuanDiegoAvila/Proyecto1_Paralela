// Manuel Archila - 161250
// Diego Franco - 20240 
// Juan Diego Avila - 20090

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define WIDTH 640
#define HEIGHT 480

typedef struct {
    float x, y;
    float dx, dy;
    SDL_Color color;
    int radio;
} Circle;

void moveCircle(Circle *circle) {
    circle->x += circle->dx;
    circle->y += circle->dy;

    if (circle->x - circle->radio <= 0 || circle->x + circle->radio >= WIDTH) circle->dx = -circle->dx;
    if (circle->y - circle->radio <= 0 || circle->y + circle->radio >= HEIGHT) circle->dy = -circle->dy;
}

// Comprobar si dos círculos se superponen
bool checkOverlap(Circle c1, Circle c2) {
    float dx = c1.x - c2.x;
    float dy = c1.y - c2.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (c1.radio + c2.radio);
}

void drawCircle(SDL_Renderer* renderer, Circle circle) {
    int x_center = circle.x;
    int y_center = circle.y;
    int r = circle.radio;

    for (int w = 0; w < r* 2; w++){
        for (int h = 0; h < r*2; h++){
            int dx = r - w; // horizontal offset
            int dy = r - h; // vertical offset
            if ((dx*dx + dy*dy) <= (r*r)) {
                SDL_RenderDrawPoint(renderer, x_center + dx, y_center + dy);
            }
        }
    }
}

void fillCircle(SDL_Renderer* renderer, int x_center, int y_center, int r) {
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
// Funcion que dibuja una cara sonriente
void drawSmileyFace(SDL_Renderer* renderer, Circle circle) {
    drawCircle(renderer, circle);
    
    int eyeRadius = circle.radio / 8;
    int eyeX1 = circle.x - circle.radio / 3;
    int eyeY1 = circle.y - circle.radio / 3;
    int eyeX2 = circle.x + circle.radio / 3;
    int eyeY2 = circle.y - circle.radio / 3;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    fillCircle(renderer, eyeX1, eyeY1, eyeRadius);
    fillCircle(renderer, eyeX2, eyeY2, eyeRadius);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = -circle.radio / 3; i < circle.radio / 3; i++) {
        int x = circle.x + i;
        int y = circle.y + circle.radio / 4 + sqrt(pow(circle.radio / 3, 2) - pow(i, 2)) / 2;
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_circles>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Circle circles[n];
    srand(time(NULL));
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
    Uint32 startTime = SDL_GetTicks();
    Uint32 lastTime = startTime;
    int frameCount = 0;

    while (running) {
        startTime = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (checkOverlap(circles[i], circles[j])) {
                // Cambio de color y ajuste de direcciones para el rebote
                circles[i].color = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255};
                circles[j].color = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255};

                // Intercambiar direcciones (esto es un modelo muy simple de "rebote")
                float temp_dx = circles[i].dx;
                float temp_dy = circles[i].dy;
                circles[i].dx = circles[j].dx;
                circles[i].dy = circles[j].dy;
                circles[j].dx = temp_dx;
                circles[j].dy = temp_dy;
            }
        }
    }

        for (int i = 0; i < n; i++) {
            SDL_SetRenderDrawColor(renderer, circles[i].color.r, circles[i].color.g, circles[i].color.b, circles[i].color.a);
            drawSmileyFace(renderer, circles[i]);  // Dibujo de caras sonrientes
            moveCircle(&circles[i]);
        }

        SDL_RenderPresent(renderer);

        frameCount++;
        if (frameCount % 30 == 0) {
            Uint32 currentTime = SDL_GetTicks();
            double timeDiff = (currentTime - lastTime);
            printf("FPS: %f\n", 30.0/ (timeDiff / 1000.0));
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

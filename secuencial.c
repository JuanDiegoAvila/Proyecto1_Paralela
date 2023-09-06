// Manuel Archila - 161250
// Diego Franco - 20240 
// Juan Diego Avila - 20090

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

// Estructura de los circulos
typedef struct {
    float x, y;
    float dx, dy;
    SDL_Color color;
    int radio;
} Circle;

// Funcion para mover los circulos sin que se salgan de la pantalla
void moveCircle(Circle *circle) {
    circle->x += circle->dx;
    circle->y += circle->dy;

    if (circle->x - circle->radio <= 0) {
        circle->dx = fabs(circle->dx);  
        circle->x = circle->radio; 
    }
    if (circle->x + circle->radio >= WIDTH) {
        circle->dx = -fabs(circle->dx);  
        circle->x = WIDTH - circle->radio; 
    }
    if (circle->y - circle->radio <= 0) {
        circle->dy = fabs(circle->dy); 
        circle->y = circle->radio;  
    }
    if (circle->y + circle->radio >= HEIGHT) {
        circle->dy = -fabs(circle->dy);  
        circle->y = HEIGHT - circle->radio;  
    }

}

// Comprobar si dos círculos se superponen
bool checkOverlap(Circle c1, Circle c2) {
    float dx = c1.x - c2.x;
    float dy = c1.y - c2.y;
    float distance = sqrt(dx * dx + dy * dy);
    return distance < (c1.radio + c2.radio); // Si la distancia es menor el tamaño de los dos círculos.
}

// Funcion para dibujar los circulos
void drawCircle(SDL_Renderer* renderer, Circle circle) {
    int x_center = circle.x;
    int y_center = circle.y;
    int r = circle.radio;

    for (int w = 0; w < r* 2; w++){
        for (int h = 0; h < r*2; h++){
            int dx = r - w; // horizontal offset
            int dy = r - h; // vertical offset
            if ((dx*dx + dy*dy) <= (r*r)) {
                SDL_RenderDrawPoint(renderer, x_center + dx, y_center + dy); // Dibujar cada pixel
            }
        }
    }
}

// Funcion para llenar de un color los circulos
void fillCircle(SDL_Renderer* renderer, int x_center, int y_center, int r) {
    for (int w = 0; w < r * 2; w++) {
        for (int h = 0; h < r * 2; h++) {
            int dx = r - w; // horizontal offset
            int dy = r - h; // vertical offset
            if ((dx * dx + dy * dy) <= (r * r)) {
                SDL_RenderDrawPoint(renderer, x_center + dx, y_center + dy); // Dibujar cada pixel
            }
        }
    }
}

// Funcion que dibuja una cara sonriente
void drawSmileyFace(SDL_Renderer* renderer, Circle circle) {
    drawCircle(renderer, circle); // Manda a dibujar el circulo completo
    
    int eyeRadius = circle.radio / 8;
    int eyeX1 = circle.x - circle.radio / 3;
    int eyeY1 = circle.y - circle.radio / 3;
    int eyeX2 = circle.x + circle.radio / 3;
    int eyeY2 = circle.y - circle.radio / 3;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Cambia el color a negro 
    fillCircle(renderer, eyeX1, eyeY1, eyeRadius); // Pinta los dos ojos 
    fillCircle(renderer, eyeX2, eyeY2, eyeRadius);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = -circle.radio / 3; i < circle.radio / 3; i++) { // Pintar la sonrisa
        int x = circle.x + i;
        int y = circle.y + circle.radio / 4 + sqrt(pow(circle.radio / 3, 2) - pow(i, 2)) / 2;
        SDL_RenderDrawPoint(renderer, x, y);
    }
}

// Funcion que verifica si el que se va crear va a caer se traslapa con otros circulos que hay en pantalla 
bool overlapsWithAny(Circle newCircle, Circle circles[], int count) {
    for (int i = 0; i < count; i++) {
        if (checkOverlap(newCircle, circles[i])) {
            return true;
        }
    }
    return false; // Si ninguno tiene overlap
}


int main(int argc, char* argv[]) {
    if (argc != 2) { // Verificacion de argumentos
        printf("Usage: %s <number_of_circles>\n", argv[0]);
        return 1;
    }
    clock_t start, end;
    double cpu_time_used;

    int n = atoi(argv[1]);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Screensaver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Circle circles[n];
    srand(time(NULL)); // Crear todos los circulos con valores random antes de verificar propiedades.
    for (int i = 0; i < n; i++) {
        circles[i] = (Circle){
            rand() % WIDTH, rand() % HEIGHT, // Posicion inicial
            (rand() % 5 + 1) * (rand() % 2 ? 1 : -1),
            (rand() % 5 + 1) * (rand() % 2 ? 1 : -1),
            {rand() % 256, rand() % 256, rand() % 256, 255}, // Color aleatorio para cada circulo
            rand() % 50 + 10 // Radio del circulo
        };
    }

    bool running = true;
    Uint32 startTime = SDL_GetTicks();
    Uint32 lastTime = startTime;
    int frameCount = 0;
    bool todos = false;
    int circulos = 0;
    Uint32 inicio = SDL_GetTicks();

    while (!todos) { // Cuando estan todos renderizados termina
        startTime = SDL_GetTicks();
        double timeDiff = (startTime - lastTime);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < circulos; i++) { // Calcula rebotes y los cambios de direccion a la hora de la colision
            for (int j = i + 1; j < circulos; j++) {
                if (checkOverlap(circles[i], circles[j])) {
                    // Cambio de color y ajuste de direcciones para el rebote
                    circles[i].color = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255};
                    circles[j].color = (SDL_Color){rand() % 256, rand() % 256, rand() % 256, 255};

                    float dx = circles[j].x - circles[i].x;
                    float dy = circles[j].y - circles[i].y;
                    float distance = sqrt(dx * dx + dy * dy);
                    float overlap = (circles[i].radio + circles[j].radio) - distance;

                    dx /= distance;
                    dy /= distance;

                    circles[i].x -= overlap * dx / 2;
                    circles[i].y -= overlap * dy / 2;
                    circles[j].x += overlap * dx / 2;
                    circles[j].y += overlap * dy / 2;
                }
            }
        }



        for (int i = 0; i < circulos; i++) { // Dibujar los circulos actuales en pantalla
            SDL_SetRenderDrawColor(renderer, circles[i].color.r, circles[i].color.g, circles[i].color.b, circles[i].color.a);
            drawSmileyFace(renderer, circles[i]);  // Dibujo de caras sonrientes
            moveCircle(&circles[i]);
        }

        SDL_RenderPresent(renderer);

        if (timeDiff > 2000.0 ) { // Valor para esperar cierto tiempo antes de agregar otro circulo
            int maxRadius = 50;
            Circle newCircle = { 
                .radio = rand() % maxRadius + 10,
                .x = rand() % (WIDTH - 2 * newCircle.radio) + newCircle.radio,
                .y = rand() % (HEIGHT - 2 * newCircle.radio) + newCircle.radio,
                .dx = (rand() % 5 + 1) * (rand() % 2 ? 1 : -1),
                .dy = (rand() % 5 + 1) * (rand() % 2 ? 1 : -1),
                .color = {rand() % 256, rand() % 256, rand() % 256, 255}
            };

            while (overlapsWithAny(newCircle, circles, circulos)) { // hasta que no haya colision no se puede crear 
                newCircle.x = rand() % (WIDTH - 2 * newCircle.radio) + newCircle.radio;
                newCircle.y = rand() % (HEIGHT - 2 * newCircle.radio) + newCircle.radio;
            }
            
            circles[circulos] = newCircle; // se cambia el circulo existente por el nuevo.

            circulos++;

            if (circulos >= n) { // ya termino de renderizar todos los circulos.
                todos = true;
            }

            lastTime = startTime;
        } 

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) todos = true;  // Salir si se cierra la ventana
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < circulos; i++) { // renderizar todos los circulos actuales en pantalla por si se agrego uno nuevo
            SDL_SetRenderDrawColor(renderer, circles[i].color.r, circles[i].color.g, circles[i].color.b, circles[i].color.a);
            drawSmileyFace(renderer, circles[i]);
            moveCircle(&circles[i]);
        }

        SDL_RenderPresent(renderer);

        frameCount++; // se calculan los frames actuales.
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - inicio;

        //printf("deltaTime: %d\n", deltaTime);
        // Si ha pasado 1 segundo, calcula los FPS
        if (deltaTime >= 1000) {
            float fps = frameCount / (deltaTime / 1000.0f);
            printf("FPS: %.2f\n", fps);

            // Reinicia las variables
            inicio = currentTime;
            frameCount = 0;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("El programa tomó %f segundos en ejecutarse.\n", cpu_time_used);


    return 0;
}

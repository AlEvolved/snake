#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h> 



int main() {
    // Initialisation de SDL et SDL_ttf
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // Création de la fenêtre et du rendu
    SDL_Window *window = SDL_CreateWindow("Snake Game",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Chargement de la police
    TTF_Font *font = TTF_OpenFont("Popcat.otf", 24);  // Remplacez par votre chemin de police
    if (font == NULL) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Initialisation du serpent et de la nourriture
    Snake snake;
    Food food;
    int score = 0;

    initSnake(&snake);
    initFood(&food, &snake);

    // Variables de contrôle du jeu
    int running = 1;
    SDL_Event e;
    srand(time(NULL));

    // Boucle principale du jeu
   while (running) {
    // Gérer les événements
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = 0;
        }
        handleInput(&e, &snake);
    }

    // Déplacer le serpent
    moveSnake(&snake);

    // Vérifier la collision
    if (checkCollision(&snake)) {
        drawGameOver(renderer, font);  // Afficher "Game Over"
        running = 0;
    }

    // Vérifier si le serpent a mangé la nourriture
    if (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y) {
        growSnake(&snake);
        initFood(&food, &snake);
        score++;  // Augmenter le score

        // Vérifier si le serpent a atteint la longueur maximale
        if (snake.length >= MAX_SNAKE_LENGTH) {
            drawWin(renderer, font);  // Afficher "You Win!"
            running = 0;  // Arrêter le jeu
        }
    }

    // Dessiner le jeu
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Couleur d'arrière-plan : noir
    SDL_RenderClear(renderer);

    draw(renderer, &snake, &food);  // Dessiner le serpent et la nourriture
    drawScore(renderer, font, score);  // Afficher le score

    SDL_RenderPresent(renderer);  // Présenter l'écran

    SDL_Delay(100);  // Contrôle de la vitesse du jeu
}


    // Nettoyer les ressources avant de quitter
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

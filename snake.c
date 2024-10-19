#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>



// Initialiser le serpent
void initSnake(Snake *snake) {
    snake->length = INITIAL_SNAKE_LENGTH;
    snake->dx = 1;
    snake->dy = 0;

    // Créer le corps du serpent : chaque segment est positionné derrière la tête
    for (int i = 0; i < snake->length; i++) {
        snake->body[i].x = INITIAL_SNAKE_LENGTH - i;
        snake->body[i].y = 10;
    }
}



// Initialiser la nourriture à une position aléatoire qui n'est pas sur le serpent ou les murs
void initFood(Food *food, Snake *snake) {
    int valid = 0;
    while (!valid) {
        food->position.x = rand() % ((WIDTH / BLOCK_SIZE) - 2) + 1; 
        food->position.y = rand() % ((HEIGHT / BLOCK_SIZE) - 2) + 1; 
        valid = 1;

        for (int i = 0; i < snake->length; i++) {
            if (snake->body[i].x == food->position.x && snake->body[i].y == food->position.y) {
                valid = 0;  // Si la nourriture est sur le serpent, on recommence
                break;
            }
        }
    }
}


// Déplacer le serpent dans la direction actuelle
void moveSnake(Snake *snake) {
    Coord newHead = snake->body[0];  // La tête actuelle
    newHead.x += snake->dx;  // Mouvement en x (horizontal)
    newHead.y += snake->dy;  // Mouvement en y (vertical)

    // Déplacer chaque segment du serpent vers l'avant (les segments suivent la tête)
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }

    // Mettre à jour la position de la tête
    snake->body[0] = newHead;
}

// Vérifier si le serpent touche un mur ou se mord lui-même
int checkCollision(Snake *snake) {
    // Si la tête du serpent touche un mur (bords de la fenêtre), c'est une collision
    if (snake->body[0].x <= 0 || snake->body[0].x >= (WIDTH / BLOCK_SIZE) - 1 ||
        snake->body[0].y <= 0 || snake->body[0].y >= (HEIGHT / BLOCK_SIZE) - 1) {
        return 1;  // Collision avec un mur
    }

    // Vérifier si la tête touche le corps
    for (int i = 1; i < snake->length; i++) {
        if (snake->body[0].x == snake->body[i].x && snake->body[0].y == snake->body[i].y) {
            return 1;  // Collision avec le corps
        }
    }
    return 0;  // Pas de collision
}


// Faire grandir le serpent lorsqu'il mange de la nourriture
void growSnake(Snake *snake) {
    snake->length++;
    snake->body[snake->length - 1] = snake->body[snake->length - 2];
}

// Dessiner le serpent, la nourriture, et les murs
void draw(SDL_Renderer *renderer, Snake *snake, Food *food) {
    // Couleur des murs : bleu
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    // Dessiner les murs en haut et en bas
    for (int x = 0; x < WIDTH / BLOCK_SIZE; x++) {
        SDL_Rect wallTop = {x * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &wallTop);
        
        SDL_Rect wallBottom = {x * BLOCK_SIZE, (HEIGHT / BLOCK_SIZE - 1) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &wallBottom);
    }

    // Dessiner les murs à gauche et à droite
    for (int y = 0; y < HEIGHT / BLOCK_SIZE; y++) {
        SDL_Rect wallLeft = {0, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &wallLeft);
        
        SDL_Rect wallRight = {(WIDTH / BLOCK_SIZE - 1) * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &wallRight);
    }

    // Couleur du serpent : blanc
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Dessiner chaque segment du serpent
    for (int i = 0; i < snake->length; i++) {
        SDL_Rect rect = {snake->body[i].x * BLOCK_SIZE, snake->body[i].y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    // Couleur de la nourriture : rouge
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = {food->position.x * BLOCK_SIZE, food->position.y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE};
    SDL_RenderFillRect(renderer, &rect);
}


// Gérer les touches pressées pour changer la direction du serpent
void handleInput(SDL_Event *e, Snake *snake) {
    if (e->type == SDL_KEYDOWN) {
        switch (e->key.keysym.sym) {
            case SDLK_UP:
                if (snake->dy == 0) {  // Si on ne va pas déjà verticalement
                    snake->dx = 0;
                    snake->dy = -1;  // Aller vers le haut
                }
                break;
            case SDLK_DOWN:
                if (snake->dy == 0) {  // Si on ne va pas déjà verticalement
                    snake->dx = 0;
                    snake->dy = 1;  // Aller vers le bas
                }
                break;
            case SDLK_LEFT:
                if (snake->dx == 0) {  // Si on ne va pas déjà horizontalement
                    snake->dx = -1;  // Aller à gauche
                    snake->dy = 0;
                }
                break;
            case SDLK_RIGHT:
                if (snake->dx == 0) {  // Si on ne va pas déjà horizontalement
                    snake->dx = 1;  // Aller à droite
                    snake->dy = 0;
                }
                break;
        }
    }
}
void drawScore(SDL_Renderer *renderer, TTF_Font *font, int score) {
    SDL_Color white = {255, 255, 255};  // Couleur blanche pour le texte
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);  // Convertir le score en texte

    // Créer une surface avec le texte
    SDL_Surface *surface = TTF_RenderText_Solid(font, scoreText, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Définir la position où afficher le texte (par exemple, sur le mur du haut)
    SDL_Rect textRect = {10, 10, surface->w, surface->h};  // Positionner à (10, 10)
    
    // Dessiner le texte
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Nettoyer la surface et la texture
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawWin(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color green = {0, 255, 0};  // Couleur verte pour "You Win!"
    const char* winText = "You Win!";

    // Créer une surface avec le texte "You Win!"
    SDL_Surface *surface = TTF_RenderText_Solid(font, winText, green);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Obtenir la taille du texte et le centrer
    SDL_Rect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;
    textRect.x = (WIDTH - textRect.w) / 2;
    textRect.y = (HEIGHT - textRect.h) / 2;

    // Dessiner le texte à l'écran
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Nettoyer la surface et la texture
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Afficher le rendu
    SDL_RenderPresent(renderer);

    // Attendre 2 secondes avant de quitter
    SDL_Delay(2000);
}


void drawGameOver(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Color red = {255, 0, 0};  // Couleur rouge pour le message "Game Over"
    const char* gameOverText = "Game Over";  // Texte à afficher

    // Créer une surface avec le texte "Game Over"
    SDL_Surface *surface = TTF_RenderText_Solid(font, gameOverText, red);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Obtenir la taille du texte et positionner le message au centre
    SDL_Rect textRect;
    textRect.w = surface->w;
    textRect.h = surface->h;
    textRect.x = (WIDTH - textRect.w) / 2;  // Centrer horizontalement
    textRect.y = (HEIGHT - textRect.h) / 2;  // Centrer verticalement

    // Dessiner le texte à l'écran
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    // Nettoyer la surface et la texture
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Afficher le rendu avec le texte "Game Over"
    SDL_RenderPresent(renderer);

    // Attendre 2 secondes avant de quitter
    SDL_Delay(2000);
}
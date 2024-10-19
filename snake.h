#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define WIDTH 640
#define HEIGHT 480
#define BLOCK_SIZE 20
#define INITIAL_SNAKE_LENGTH 5
#define MAX_SNAKE_LENGTH 50  


typedef struct {
    int x, y;
} Coord;

typedef struct {
    Coord body[100];  
    int length;       
    int dx, dy;       
} Snake;


typedef struct {
    Coord position;  
} Food;

// Function prototypes
void initSnake(Snake *snake);
void initFood(Food *food, Snake *snake);
void moveSnake(Snake *snake);
int checkCollision(Snake *snake);
void growSnake(Snake *snake);
void draw(SDL_Renderer *renderer, Snake *snake, Food *food);
void handleInput(SDL_Event *e, Snake *snake);
void gameOver(SDL_Renderer *renderer, SDL_Window *window);
void drawScore(SDL_Renderer *renderer, TTF_Font *font, int score);
void drawGameOver(SDL_Renderer *renderer, TTF_Font *font);
void drawWin(SDL_Renderer *renderer, TTF_Font *font);

#endif


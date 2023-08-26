#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL.h>  
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "../headers/ltexture.h"

enum GRID_SPACE_FLAGS : uint16_t {
    EMPTY =      0b000001,
    BOMB  =      0b000010,
    REVEALED =   0b000100,
    FLOOD_FILL = 0b001000,
    FLAG =       0b010000,
};

struct Vec2 {
    int x;
    int y;
};

class Minesweeper {
    public:
    bool isRunning, shouldClose, leftClick = false, rightClick = false,
    hasRevealedSpace = false, hasFlaggedSpace = false, win = false,
    lose = false;
    int gridWidth, gridHeight, cellSize, bombCount;
    SDL_Renderer* renderer;
    TTF_Font* font = NULL;
    uint16_t** grid;
    uint16_t** bombCounts;
    LTexture numbers[7];
    LTexture bomb;
    LTexture flag; 
    LTexture youWin, youLose;

    Minesweeper(SDL_Renderer*);
    ~Minesweeper();

    void Init(unsigned int width, unsigned int height, unsigned int cellSize, unsigned int bombCount);
    void Tick(Uint64);
    void Render(Uint64);
    void RevealGridTiles(int xPos, int yPos);
    void MouseDown(SDL_MouseButtonEvent& event);
    void MouseUp(SDL_MouseButtonEvent& event);

    private:
    bool loadFont();
};
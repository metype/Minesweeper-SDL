#include "../headers/minesweeper.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <filesystem>

Minesweeper::Minesweeper(SDL_Renderer* renderer) {
	this->renderer = renderer;
}

Minesweeper::~Minesweeper() {
	// SDL_DestroyTexture(font);
	TTF_CloseFont( font );
}

void Minesweeper::Init(unsigned int width, unsigned int height, unsigned int cellSize, unsigned int bombCount) {
	this->isRunning = true;
	this->shouldClose = false;

	this->gridWidth = int(width/cellSize);
	this->gridHeight = int(height/cellSize);
	this->cellSize = cellSize;
	this->bombCount = bombCount;

	loadFont();

	for(int i=0; i<7; i++){
		numbers[i].loadFromRenderedText(font, renderer, std::to_string(i+1), {255,255,255,255});
	}

	bomb.loadFromRenderedText(font, renderer, "B", {255,0,0,255});
	flag.loadFromRenderedText(font, renderer, "F", {0,0,0,255});
	youWin.loadFromRenderedText(font, renderer, "You Won!", {20,255,20,255});
	youLose.loadFromRenderedText(font, renderer, "You Lost!", {255,20,20,255});

	ResetGame();
}

void Minesweeper::ResetGame() {
	this->win = false;
	this->lose = false;

	if(grid != nullptr) {
		for(int i=0; i < gridWidth; i++) {
			delete[] grid[i];
		}
		delete[] grid;
	}
	if(bombCounts != nullptr) {
		for(int i=0; i < gridWidth; i++) {
			delete[] bombCounts[i];
		}
		delete[] bombCounts;
	}

	grid = new uint16_t*[this->gridWidth];
	bombCounts = new uint16_t*[this->gridWidth];
	

	for(int x = 0; x < this->gridWidth; x++){
		grid[x] = new uint16_t[this->gridHeight];
		bombCounts[x] = new uint16_t[this->gridHeight];

		for(int y = 0; y < this->gridHeight; y++){
			grid[x][y] = EMPTY;
			bombCounts[x][y] = 0;
		}
	}

	for(int bombsPlaced = 0; bombsPlaced < bombCount;) {
		int bombXPos = rand()%gridWidth;
		int bombYPos = rand()%gridHeight;
		if(grid[bombXPos][bombYPos] & BOMB) continue;
		grid[bombXPos][bombYPos] |= BOMB;
		bombsPlaced++;
	}

	for(int x = 0; x < this->gridWidth; x++){
		for(int y = 0; y < this->gridHeight; y++){
			int bombCount = 0;
			for(int xOff = -1; xOff <= 1; xOff++) {
				for(int yOff = -1; yOff <= 1; yOff++){
					if(x+xOff < 0 || x+xOff > this->gridWidth - 1 || y + yOff < 0 || y+yOff > this->gridHeight - 1) continue;
					if(grid[x+xOff][y+yOff] & BOMB) bombCount++;
				}
			}
			bombCounts[x][y] = bombCount;
		}
	}
}

void Minesweeper::Tick(Uint64 dt) {
	if(this->shouldClose) {
		this->isRunning = false;
	}
	if(!(win | lose)) {
		bool gridHasBeenModified = false;
		int mouseX = 0;
		int mouseY = 0;
		SDL_GetMouseState(&mouseX, &mouseY);
		if(leftClick && !hasRevealedSpace) {
			int gridMouseX, gridMouseY;
			gridMouseX = (int)((mouseX+0.0f) / cellSize); // Force float division cause integer division can be nasty, so is this code tbh
			gridMouseY = (int)((mouseY+0.0f) / cellSize);

			if(!(gridMouseX < 0 || gridMouseX > this->gridWidth-1 || gridMouseY < 0 || gridMouseY > this->gridHeight-1)) {
				RevealGridTiles(gridMouseX, gridMouseY);
				gridHasBeenModified = true;
			}

			hasRevealedSpace = true;
		} else if (!leftClick) {
			hasRevealedSpace = false;
		}
		if(rightClick && !hasFlaggedSpace) {
			int gridMouseX, gridMouseY;
			gridMouseX = (int)((mouseX+0.0f) / cellSize); // Force float division cause integer division can be nasty, so is this code tbh
			gridMouseY = (int)((mouseY+0.0f) / cellSize);

			if(!(gridMouseX < 0 || gridMouseX > this->gridWidth-1 || gridMouseY < 0 || gridMouseY > this->gridHeight-1))
				if(~grid[gridMouseX][gridMouseY] & REVEALED) {
					grid[gridMouseX][gridMouseY] ^= FLAG;
					gridHasBeenModified = true;
				}

			hasFlaggedSpace = true;
		} else if (!rightClick) {
			hasFlaggedSpace = false;
		}

		if(gridHasBeenModified) {
			int flaggedBombCount = 0;
			bool onlyBombsCovered = true;
			for(int x = 0; x < this->gridWidth; x++){
				for(int y = 0; y < this->gridHeight; y++){
					if((grid[x][y] & REVEALED) && (grid[x][y] & BOMB)) lose = true;
					if((grid[x][y] & FLAG) && (grid[x][y] & BOMB)) flaggedBombCount++;
					if((~grid[x][y] & REVEALED) && (~grid[x][y] & BOMB)) onlyBombsCovered = false;
				}
			}
			if(flaggedBombCount == bombCount) win = true;
			if(onlyBombsCovered) win = true;
		}
	} else if((leftClick | rightClick) && !(hasRevealedSpace || hasFlaggedSpace)) {
		ResetGame();
		hasRevealedSpace = true;
		hasFlaggedSpace = true;
	} else if (!(leftClick | rightClick)) {
		hasRevealedSpace = false;
		hasFlaggedSpace = false;
	}
}

void Minesweeper::RevealGridTiles(int xPos, int yPos) {
	if(grid[xPos][yPos] & FLAG) return;
	grid[xPos][yPos] |= REVEALED;
	if(bombCounts[xPos][yPos] > 0) return;
	Vec2 offsets[] = {{0,-1},{-1,0},{1,0},{0,1}};
	for(int i=0; i<4; i++){
		Vec2 newPos = {xPos + offsets[i].x, yPos + offsets[i].y};
		if(newPos.x < 0 || newPos.x >= gridWidth || newPos.y < 0 || newPos.y >= gridHeight) continue;
		if(grid[newPos.x][newPos.y] & BOMB) continue;
		if(grid[newPos.x][newPos.y] & REVEALED) continue;
		RevealGridTiles(newPos.x, newPos.y);
	}
}

void Minesweeper::Render(Uint64 dt) {
	for(int x = 0; x < this->gridWidth; x++){
		for(int y = 0; y < this->gridHeight; y++){
			SDL_Rect rect = {x*this->cellSize, y*this->cellSize, this->cellSize, this->cellSize};

			// Only works cause it's a monospaced font, but why wouldn't you use a monospaced font?
			int xOff = (this->cellSize / 2.0f) - (flag.getWidth() / 2.0f);
			int yOff = (this->cellSize / 2.0f) - (flag.getHeight() / 2.0f);

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

			if(~grid[x][y] & REVEALED) {
				SDL_RenderFillRect(this->renderer, &rect);
			} else {
				SDL_RenderDrawRect(this->renderer, &rect);
				if(~grid[x][y] & BOMB) {
					numbers[bombCounts[x][y]-1].render(renderer, (x*this->cellSize) + xOff, (y*this->cellSize) + yOff);
				} else {
					bomb.render(renderer, (x*this->cellSize) + xOff, (y*this->cellSize) + yOff);
				}
			}

			if(grid[x][y] & FLAG) {
				flag.render(renderer, (x*this->cellSize) + xOff, (y*this->cellSize) + yOff);
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawRect(this->renderer, &rect);
		}
	}
	if(win) {
		int windowWidth = (this->gridWidth*this->cellSize);
		int windowHeight = (this->gridHeight*this->cellSize);
		int xOff = (windowWidth / 2.0f) - (youWin.getWidth() / 2.0f);
		int yOff = (windowHeight / 2.0f) - (youWin.getHeight() / 2.0f);

		SDL_Rect rect = {windowWidth/2 - (windowWidth/8), windowHeight/2 - (windowHeight/10), windowWidth/4, windowHeight/5};

		SDL_SetRenderDrawColor(renderer, 170, 170, 170, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(this->renderer, &rect);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(this->renderer, &rect);

		youWin.render(renderer, xOff, yOff);
	}
	if(lose) {
		int windowWidth = (this->gridWidth*this->cellSize);
		int windowHeight = (this->gridHeight*this->cellSize);
		int xOff = (windowWidth / 2.0f) - (youLose.getWidth() / 2.0f);
		int yOff = (windowHeight / 2.0f) - (youLose.getHeight() / 2.0f);

		SDL_Rect rect = {windowWidth/2 - (windowWidth/8), windowHeight/2 - (windowHeight/10), windowWidth/4, windowHeight/5};

		SDL_SetRenderDrawColor(renderer, 170, 170, 170, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(this->renderer, &rect);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(this->renderer, &rect);

		youLose.render(renderer, xOff, yOff);
	}
}

void Minesweeper::MouseDown(SDL_MouseButtonEvent &event) {
	if(event.button == SDL_BUTTON_LEFT) {
		leftClick = true;
	}
	if(event.button == SDL_BUTTON_RIGHT) {
		rightClick = true;
	}
}

void Minesweeper::MouseUp(SDL_MouseButtonEvent &event) {
	if(event.button == SDL_BUTTON_LEFT) {
		leftClick = false;
	}
	if(event.button == SDL_BUTTON_RIGHT) {
		rightClick = false;
	}
}

bool Minesweeper::loadFont()
{
	//Loading success flag
	bool success = true;

	//Open the font
	font = TTF_OpenFont( "font.ttf", this->cellSize / 2);
	if( font == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	// else
	// {
	//     //Render text
	//     SDL_Color textColor = { 0, 0, 0 };
	//     if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
	//     {
	//         printf( "Failed to render text texture!\n" );
	//         success = false;
	//     }
	// }

	return success;
}

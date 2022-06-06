#ifndef GUI_H
#define GUI_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "moves.h"
#include <string>
#include <iostream>
#include <format>

extern int board[];
extern int sideToMove;

class Gui
{
private:
	SDL_Renderer* renderer;
	SDL_Texture* piecesTextures[12];
	SDL_Texture* backgroundTexture;
	const SDL_Color light_color = { 240, 217, 181, 255 };
	const SDL_Color dark_color = { 181, 136, 99, 255 };
	const SDL_Color lightRed_color = { 248, 159, 132, 255 };
	const SDL_Color darkRed_color = { 222, 98, 71, 255 };
	const SDL_Color text_color = { 255, 255, 255, 255 };
	TTF_Font* font = TTF_OpenFont("./assets/calibril.ttf", 16);
	
	void drawBackground();
	void drawGameInfo(Moves::moveList*);
	void drawPieces();
	void drawPieces(int);
	void loadTextures();
public:
	void draw(Moves::moveList*);
	void draw(Moves::moveList*, Moves::moveList*, int);
	int getSquareAt(int, int);
	int grabPieceAt(int, int);
	void drawMoveSquares(Moves::moveList*, int);
	void drawPromotionWindow(int);

	Gui(SDL_Window*);
	~Gui();
};


#endif // !GUI_H
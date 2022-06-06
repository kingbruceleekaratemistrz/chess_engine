#include "gui.h"

void Gui::drawBackground()
{
	SDL_Rect bckgRect;
	bckgRect.x = 0;
	bckgRect.y = 0;
	bckgRect.w = 1024;
	bckgRect.h = 720;
	SDL_RenderCopy(renderer, backgroundTexture, 0, &bckgRect);

	SDL_Rect rect;
	rect.w = rect.h = 60;

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			SDL_Color color = (file + rank) % 2 ? dark_color : light_color;
			rect.x = 60 * file + 60;
			rect.y = 60 * rank + 60;

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void Gui::drawPieces()
{
	SDL_Rect pieceRect;
	pieceRect.w = 60;
	pieceRect.h = 60;

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 16 + file;
			
			// jesli na polu znajduje sie jakas figura
			if (board[square] < e)
			{
				pieceRect.x = 60 * file + 60;
				pieceRect.y = 60 * rank + 60;

				// narysuj figure
				SDL_RenderCopy(renderer, piecesTextures[board[square]], 0, &pieceRect);
			}
		}
	}
}

void Gui::drawPieces(int squareToIgnore)
{
	SDL_Rect pieceRect;
	pieceRect.w = 60;
	pieceRect.h = 60;

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 16 + file;

			// jesli na polu znajduje sie jakass figura
			if (board[square] < e && square != squareToIgnore)
			{
				pieceRect.x = 60 * file + 60;
				pieceRect.y = 60 * rank + 60;

				// narysuj figure
				SDL_RenderCopy(renderer, piecesTextures[board[square]], 0, &pieceRect);
			}
		}
	}

	int mx, my;
	SDL_GetMouseState(&mx, &my);
	pieceRect.x = mx - 30;
	pieceRect.y = my - 30;

	SDL_RenderCopy(renderer, piecesTextures[board[squareToIgnore]], 0, &pieceRect);
}

void Gui::drawMoveSquares(Moves::moveList* ml, int sourceSquare)
{
	SDL_Rect rect;
	rect.w = rect.h = 60;

	for (int i = 0; i < ml->counter; i++)
	{
		int move = ml->storedMoves[i];
		if (getMoveSoruce(move) == sourceSquare)
		{
			int targetSquare = getMoveTarget(move);
			int file = targetSquare % 8;
			int rank = targetSquare / 16;
			SDL_Color color = (file + rank) % 2 ? darkRed_color : lightRed_color;
			rect.x = 60 * file + 60;
			rect.y = 60 * rank + 60;

			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}

void Gui::loadTextures()
{
	SDL_Surface* tmpSurf;

	tmpSurf = SDL_LoadBMP("./assets/background.bmp");
	if (!tmpSurf) {
		std::cout << "Failed to load an image at path: ./assets/background.bmp" << std::endl;
		std::cout << SDL_GetError() << std::endl;
		SDL_FreeSurface(tmpSurf);
		return;
	}
	this->backgroundTexture = SDL_CreateTextureFromSurface(renderer, tmpSurf);

	std::string piecesNames[6] = { "pawn", "knight", "bishop", "rook", "queen", "king" };

	for (int i = 0; i < 12; i++)
	{
		std::string tmp_path = (i < 6) ? "./assets/white_" + piecesNames[i] + ".bmp" :
			"./assets/black_" + piecesNames[i - 6] + ".bmp";

		tmpSurf = SDL_LoadBMP(tmp_path.c_str());
		if (!tmpSurf)
		{
			std::cout << "Failed to load an image at path: " + tmp_path << std::endl;
			std::cout << SDL_GetError() << std::endl;
			SDL_FreeSurface(tmpSurf);
			return;
		}
		this->piecesTextures[i] = SDL_CreateTextureFromSurface(renderer, tmpSurf);
	}

	SDL_FreeSurface(tmpSurf);
}

void Gui::drawGameInfo(Moves::moveList* moves)
{
	std::string buffer = "";
	for (int i = 0; i < moves->counter; i++)
	{
		if (i % 2 == 0)
			buffer = std::to_string(i / 2 + 1) + ". ";
		else
			buffer.append(" ");

		int move = moves->storedMoves[i];
		int target = getMoveTarget(move);
		int source = getMoveSoruce(move);
		
		if (getCastleFlag(move))
		{
			if (target % 16 == 2)
				buffer.append("O-O-O");
			else if (target % 16 == 6)
				buffer.append("O-O");
		}
		else
		{
			buffer.append(squareToCoords[source]);

			if (getCaptureFlag(move))
				buffer.append("x");

			buffer.append(squareToCoords[target]);

			if (getPromotedPiece(move))
			{
				buffer.append("=");
				buffer.append(promPieces[getPromotedPiece(move)]);
			}
		}

		if (i % 2 == 1 || i == moves->counter - 1)
		{
			SDL_Surface* surf = TTF_RenderText_Solid(font, buffer.c_str(), text_color);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
			SDL_Rect rect;
			rect.x = 600 + (int)(i / 60) * 135;
			rect.y = 60 + ((i / 2) % 30) * surf->h;
			rect.w = surf->w;
			rect.h = surf->h;

			SDL_RenderCopy(renderer, texture, 0, &rect);
			SDL_FreeSurface(surf);
			SDL_DestroyTexture(texture);
		}
	}
}

void Gui::draw(Moves::moveList* history)
{
	drawBackground();
	drawGameInfo(history);
	drawPieces();
	SDL_RenderPresent(renderer);
}

void Gui::draw(Moves::moveList* ml, Moves::moveList* history, int movingPieceSquare)
{
	drawBackground();
	drawGameInfo(history);
	drawMoveSquares(ml, movingPieceSquare);
	drawPieces(movingPieceSquare);
	SDL_RenderPresent(renderer);
}


int Gui::getSquareAt(int mx, int my)
{
	if (mx < 60 || mx > 540 || my < 60 || my > 540)
		return no_sq;
	else
	{
		int file = (mx - 60) / 60;
		int rank = (my - 60) / 60;
		int square = rank * 16 + file;

		return square;
	}
}

int Gui::grabPieceAt(int mx, int my)
{
	int square = getSquareAt(mx, my);

	// jesli pole nie jest puste
	if (board[square] < e)
	{
		if (sideToMove == white)
		{
			if (board[square] < p)
				return square;
		}
		else if (board[square >= p])
			return square;
	}

	return no_sq;
}

void Gui::drawPromotionWindow(int targetSquare)
{
	int x = 60 * (targetSquare % 8) + 30;
	int y;
	if (targetSquare < 8)
		y = 30;
	else
		y = 450;

	SDL_Rect bckgRect;
	bckgRect.x = x;
	bckgRect.y = y;
	bckgRect.w = 120;
	bckgRect.h = 120;

	SDL_SetRenderDrawColor(renderer, light_color.r, light_color.g, light_color.b, light_color.a);
	SDL_RenderFillRect(renderer, &bckgRect);

	SDL_Rect pieceRect;
	pieceRect.w = 60;
	pieceRect.h = 60;

	int piecesIter[] = { Q, R, B, N, q, r, b, n };
	for (int i = 0; i < 4; i++)
	{
		pieceRect.x = x + (i % 2) * 60;
		pieceRect.y = y + (int)(i / 2) * 60;

		SDL_RenderCopy(renderer, piecesTextures[piecesIter[(targetSquare > 8) ? i + 4 : i]], 0, &pieceRect);
	}

	SDL_SetRenderDrawColor(renderer, dark_color.r, dark_color.g, dark_color.b, dark_color.a);
	SDL_RenderDrawLine(renderer, x, y, x + 120, y);
	SDL_RenderDrawLine(renderer, x, y+1, x + 120, y+1);

	SDL_RenderDrawLine(renderer, x, y, x, y + 120);
	SDL_RenderDrawLine(renderer, x+1, y, x+1, y + 120);

	SDL_RenderDrawLine(renderer, x + 120, y + 120, x, y + 120);
	SDL_RenderDrawLine(renderer, x + 120, y + 119, x, y + 119);

	SDL_RenderDrawLine(renderer, x + 120, y + 120, x + 120, y);
	SDL_RenderDrawLine(renderer, x + 119, y + 120, x + 119, y);

	SDL_RenderDrawLine(renderer, x + 59, y, x + 59, y + 120);
	SDL_RenderDrawLine(renderer, x + 60, y, x + 60, y + 120);
	SDL_RenderDrawLine(renderer, x + 61, y, x + 61, y + 120);

	SDL_RenderDrawLine(renderer, x, y + 59, x + 120, y + 59);
	SDL_RenderDrawLine(renderer, x, y + 60, x + 120, y + 60);
	SDL_RenderDrawLine(renderer, x, y + 61, x + 120, y + 61);


	SDL_RenderPresent(renderer);
}

Gui::Gui(SDL_Window* win)
{
	renderer = SDL_CreateRenderer(win, -1, 0);
	loadTextures();
}

Gui::~Gui()
{
	SDL_DestroyTexture(backgroundTexture);
	for (int i = 0; i < 12; i++)
		SDL_DestroyTexture(piecesTextures[i]);
}
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"
#include "moves.h"
#include "gui.h"

int DEPTH = 5;

// reprezentacja szchownicy
int board[128] = {
r, n, b, q, k, b, n, r,  o, o, o, o, o, o, o, o,
p, p, p, p, p, p, p, p,  o, o, o, o, o, o, o, o,
e, e, e, e, e, e, e, e,  o, o, o, o, o, o, o, o,
e, e, e, e, e, e, e, e,  o, o, o, o, o, o, o, o,
e, e, e, e, e, e, e, e,  o, o, o, o, o, o, o, o,
e, e, e, e, e, e, e, e,  o, o, o, o, o, o, o, o,
P, P, P, P, P, P, P, P,  o, o, o, o, o, o, o, o,
R, N, B, Q, K, B, N, R,  o, o, o, o, o, o, o, o
};

// informacje o szachownicy
int sideToMove = white;
int enpassantSquare = no_sq;
int castle = 15;
int playerSide = white;

// pola zajmowane przez królów
int kingSquare[2] = { e1, e8 };

// zmienne do przechowywania kopii informacji o szachownicy
int board_copy[128], kingSquare_copy[2];
int sideToMove_copy, enpassantSquare_copy, castle_copy;

void debug_printBoard();
void debug_printMoveList(Moves::moveList*);
void debug_printMove(int);

int main(int argc, char* argv[])
{
	// inicjalizacja SDL i okna
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 720, SDL_WINDOW_SHOWN);

	// inicjalizacja SDL_ttf
	if (TTF_Init() == -1)
	{
		std::cerr << "Nie udalo sie zainicjalizowac SDL_ttf.\n";
		return -1;
	}

	srand(time(NULL));

	// odczyt informacji o szachownicy z pozycji FEN
	Board::parseFen(startPosition);

	// inicjalizacja gui
	Gui g = Gui(window);

	// zmienne do obs³ugi pêtli programu
	bool isRunning = true;
	bool update = true;
	int draggedPieceSquare = no_sq;
	SDL_Event ev;
	time_t time1, time2;
	
	// lista do przechowywania ruchow
	Moves::moveList moveList[1];
	moveList->counter = 0;
	Moves::generateMoves(moveList);
	Moves::sortMoves(moveList);
	// lista do prechowywania wykonanych ruchów
	Moves::moveList moveHistory[1];
	moveHistory->counter = 0;

	// g³ówna pêtla programu
	while (isRunning)
	{
		if (sideToMove == playerSide)
		{
			time(&time1);

			// obs³uga zdarzeñ
			while (SDL_PollEvent(&ev) != 0)
			{
				if (ev.type == SDL_QUIT)
					isRunning = false;
				else if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					// uzyskanie wspolrzednych myszki
					int mx, my;
					SDL_GetMouseState(&mx, &my);

					// uzyskanie pola nad ktorym znajduje sie mysz podczas klikniecia
					// jesli na tym polu znajduje sie figura koloru wykonujacego ruch
					draggedPieceSquare = g.grabPieceAt(mx, my);

					// jesli natrafiono na figure odpowiedniego koloru nastapi aktualizacja sceny
					update = (draggedPieceSquare != no_sq) ? true : false;
				}
				else if (ev.type == SDL_MOUSEBUTTONUP)
				{
					// uzyskanie wspolrzednych myszki
					int mx, my;
					SDL_GetMouseState(&mx, &my);

					// uzyslanie numeru pola na ktore figura zostala odlozona
					int targetSquare = g.getSquareAt(mx, my);

					// jesli ruch to promocja to wyswietl okno wyboru
					int promotedPieceType = e;
					if ((targetSquare < 8 && board[draggedPieceSquare] == P) || (targetSquare > 111 && board[draggedPieceSquare] == p))
					{
						// sprawdzenie czy ruch z promocja jest legalny 
						if (Moves::isInMoveList(moveList, Moves::encodeMove(draggedPieceSquare, targetSquare, (sideToMove == white) ? Q : q)))
						{
							g.drawPromotionWindow(targetSquare);

							// wspolrzedne okna promocji
							int wx = 60 * (targetSquare % 8) + 30;
							int wy;
							if (targetSquare < 8)
								wy = 30;
							else
								wy = 450;

							SDL_PollEvent(&ev);
							while (true)
							{
								if (ev.type == SDL_QUIT)
								{
									isRunning = false;
									break;
								}
								else if (ev.type == SDL_MOUSEBUTTONDOWN)
								{
									int mx, my;
									SDL_GetMouseState(&mx, &my);

									// lewa kolumna
									if (mx >= wx && mx < wx + 60)
									{
										// lewy gorny
										if (my >= wy && my < wy + 60)
										{
											promotedPieceType = (sideToMove == white) ? Q : q;
											break;
										}
										// lewy dolny
										else if (my > wy + 60 && my <= wy + 120)
										{
											promotedPieceType = (sideToMove == white) ? B : b;
											break;
										}
									}
									// prawa kolumna
									else if (mx > wx + 60 && mx <= wx + 120)
									{
										// prawy gorny
										if (my >= wy && my < wy + 60)
										{
											promotedPieceType = (sideToMove == white) ? R : r;
											break;
										}
										// prawy dolny
										else if (my > wy + 60 && my <= wy + 120)
										{
											promotedPieceType = (sideToMove == white) ? N : n;
											break;
										}
									}
									else
									{
										promotedPieceType = e;
										break;
									}
								}
								SDL_PollEvent(&ev);
							}

						}
					}

					// zakodowanie ruchu
					int move = Moves::encodeMove(draggedPieceSquare, targetSquare, promotedPieceType);

					// jesli ruch jest legalny wykonaj ruch
					if (Moves::isInMoveList(moveList, move))
					{
						Moves::makeMove(move, allMoves);
						moveHistory->storedMoves[moveHistory->counter++] = move;
					}

					draggedPieceSquare = no_sq;
				}
			}

			// update sceny
			if (update)
			{
				if (draggedPieceSquare != no_sq)
				{
					// rysowanie figur na ich miejscach oraz figury przemieszczanej przez myszke
					g.draw(moveList, moveHistory, draggedPieceSquare);

					// opoznienie w celu uzyskania 60 fps
					time(&time2);
					Uint32 diff = 16.6667f - difftime(time2, time1);
					if (diff > 0)
						SDL_Delay(diff);
				}
				else
				{
					g.draw(moveHistory);					
					debug_printBoard();	
					Moves::searchPosition(DEPTH);
					moveList->counter = 0;
					Moves::generateMoves(moveList);
					Moves::sortMoves(moveList);
					if (moveList->counter == 0)
					{
						printf("Gracz (%s) wygral!\n", (playerSide == white) ? "bialy" : "czarny");
						isRunning = false;
					}
					update = false;
				}
			}
		}
		else
		{
			Moves::searchPosition(DEPTH);
			Moves::makeMove(Moves::bestMove, allMoves);
			moveHistory->storedMoves[moveHistory->counter++] = Moves::bestMove;
			g.draw(moveHistory);
			debug_printBoard();
			//Moves::searchPosition(DEPTH);
			moveList->counter = 0;
			Moves::generateMoves(moveList);
			//Moves::sortMoves(moveList);
			if (moveList->counter == 0)
			{
				printf("AI (%s) wygralo!\n", (playerSide == black) ? "bialy" : "czarny");
				isRunning = false;
			}
		}
	}

	TTF_Quit();
	SDL_Quit();
	return 0;
}

void debug_printBoard()
{
	system("cls");

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 16 + file;

			if (file == 0)
				std::cout << 8 - rank << "  ";

			std::cout << asciiPieces[board[square]] << " ";
		}

		std::cout << std::endl;
	}

	char castleStr[5] = { '-', '-', '-', '-', 0 };
	if (castle & qc) castleStr[0] = 'q';
	if (castle & kc) castleStr[1] = 'k';
	if (castle & Qc) castleStr[2] = 'Q';
	if (castle & Kc) castleStr[3] = 'K';


	std::cout << std::endl << "   a b c d e f g h" << std::endl;

	std::cout << std::endl << "   -------------------";
	std::cout << std::endl << "   sideToMove:   " << ((sideToMove == 0) ? "white" : "black");
	std::cout << std::endl << "   enpassantSquare: " << ((enpassantSquare == no_sq) ? "--" : squareToCoords[enpassantSquare]);
	std::cout << std::endl << "   castleRigths:  " << castleStr;
	std::cout << std::endl << "   King square: " << squareToCoords[kingSquare[sideToMove]];
	std::cout << std::endl << "   Evaluation: " << Moves::evaluate() << std::endl << std::endl;
	//std::cout << std::endl << "   Best move: " << squareToCoords[getMoveSoruce(Moves::bestMove)] << squareToCoords[getMoveTarget(Moves::bestMove)] << std::endl << std::endl;
}

void debug_printMoveList(Moves::moveList* ml)
{
	printf("\nGenerated moves for: %s\n\n", (sideToMove == white) ? "white" : "black");
	for (int i = 0; i < ml->counter; i++)
		debug_printMove(ml->storedMoves[i]);
	printf("\n");
}

void debug_printMove(int move)
{
	printf("%s%s %c %i%i%i%i\n", 
		squareToCoords[getMoveSoruce(move)],
		squareToCoords[getMoveTarget(move)],
		promotedPieces[getPromotedPiece(move)],
		getCaptureFlag(move),
		getDoublePawnMoveFlag(move),
		getEnpassantFlag(move),
		getCastleFlag(move)
	);
}

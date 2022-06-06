#ifndef BOARD_H
#define BOARD_H

#include "enumerations.h"
#include <iostream>
#include <string.h>

extern int board[];
extern int sideToMove;
extern int enpassantSquare;
extern int castle;
extern int kingSquare[];

class Board
{
private:
	static void resetBoard();
	static int pieceCharToEnum(char);
public:
	static void parseFen(char*);
};

#endif // !BOARD_INITIALIZER_H
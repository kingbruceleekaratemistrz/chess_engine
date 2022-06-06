#ifndef MOVES_H
#define MOVES_H

#include <stdio.h>
#include "enumerations.h"
#include "board.h"

extern int board[];
extern int sideToMove;
extern int enpassantSquare;
extern int castle;

class Moves
{
public:
	typedef struct {
		int storedMoves[256];
		int counter;
	} moveList;
	static int ply;
	static int bestMove;
	static long nodes;
private:
	static bool isMoveLegal(int);
public:
	static void generateMoves(moveList*);
	static int makeMove(int, int);
	static int isSquareAttacked(int, int);
	static void addMoves(moveList*, int);
	static int encodeMove(int, int, int);
	static int evaluate();
	static int negamax(int, int, int);
	static void searchPosition(int);
	static int quiescence(int, int);
	static int scoreMove(int);
	static int sortMoves(moveList*);

	static bool isInMoveList(moveList*, int);
};

#endif // !MOVES_H

#include "moves.h"

int Moves::ply = 0;
int Moves::bestMove;
long Moves::nodes = 0;

int inline abs(int v)
{
	if (v > 0)
		return v;
	else
		return (-1 * v);
}

bool Moves::isInMoveList(Moves::moveList* ml, int move)
{
	for (int i = 0; i < ml->counter; i++)
	{
		if (ml->storedMoves[i] == move)
			return true;
	}

	return false;
}

void Moves::generateMoves(moveList* moves)
{
	moves->counter = 0;

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 16 + file;

			// biayy pionek i roszada
			if (!sideToMove)
			{
				// bialy pionek
				if (board[square] == P)
				{
					// ruchy pionka - bez bicia
					int targetSquare = square - 16;
					// sprawdzenie czy dane pole jest na szachownicy i czy jest puste
					if (!((targetSquare) & 0x88) && board[targetSquare] == e)
					{	
						// promocja pionka
						if (rank == 1)
						{
							Moves::addMoves(moves, setMove(square, targetSquare, Q, 0, 0, 0, 0));
							Moves::addMoves(moves, setMove(square, targetSquare, R, 0, 0, 0, 0));
							Moves::addMoves(moves, setMove(square, targetSquare, B, 0, 0, 0, 0));
							Moves::addMoves(moves, setMove(square, targetSquare, N, 0, 0, 0, 0));
						}
						else
						{
							// ruch o pole do przodu
							Moves::addMoves(moves, setMove(square, targetSquare, 0, 0, 0, 0, 0));
							// ruch o dwa pola do przodu
							if (rank == 6 && board[square - 32] == e)
								Moves::addMoves(moves, setMove(square, square - 32, 0, 0, 1, 0, 0));
						}
					}

					// ruchy pionka - z biciem
					for (int i = 0; i < 2; i++)
					{
						int pawnOffset = bishopOffsets[i];
						int targetSquare = square + pawnOffset;

						if (!(targetSquare & 0x88))
						{
							// bicia i bicia z promocja
							if (board[targetSquare] >= p && board[targetSquare] < e)
							{
								if (rank == 1)
								{
									Moves::addMoves(moves, setMove(square, targetSquare, Q, 1, 0, 0, 0));
									Moves::addMoves(moves, setMove(square, targetSquare, R, 1, 0, 0, 0));
									Moves::addMoves(moves, setMove(square, targetSquare, B, 1, 0, 0, 0));
									Moves::addMoves(moves, setMove(square, targetSquare, N, 1, 0, 0, 0));
								}
								else
									Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 0, 0));
							}

							// bicie w przelocie
							if (targetSquare == enpassantSquare)
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 1, 0));
						}
					}
				}

				// biale roszada
				if (board[square] == K)
				{
					// jesli krotka roszada jest dostepna
					if (castle & Kc)
					{
						if (board[f1] == e && board[g1] == e)
						{
							// spradzanie czy krol oraz f1 nie sa atakowane
							if (!isSquareAttacked(e1, black) && !isSquareAttacked(f1, black))
								Moves::addMoves(moves, setMove(e1, g1, 0, 0, 0, 0, 1));
						}
					}
					// jesli dluga roszada jest dostepna
					if (castle & Qc)
					{
						if (board[d1] == e && board[c1] == e && board[b1] == e)
						{
							// sprawdzanie czy ktorl oraz d1 nei sa atakowane
							if (!isSquareAttacked(e1, black) && !isSquareAttacked(d1, black))
								Moves::addMoves(moves, setMove(e1, c1, 0, 0, 0, 0, 1));
						}
					}
				}
			}
			// czarny pionek i roszada
			else
			{
				// czarny pionek
				if (board[square] == p)
				{
					// ruchy pionka - bez bicia
					int targetSquare = square + 16;
					// sprawdzenie czy dane pole jest na szachownicy i czy jest puste
					if (!((targetSquare) & 0x88) && board[targetSquare] == e)
					{
						// promocje pionka
						if (rank == 6)
						{
							Moves::addMoves(moves, setMove(square, targetSquare, q, 0, 0, 0, 0));
							Moves::addMoves(moves, setMove(square, targetSquare, r, 0, 0, 0, 0));
							Moves::addMoves(moves, setMove(square, targetSquare, b, 0, 0, 0, 0));
							Moves::addMoves(moves, setMove(square, targetSquare, n, 0, 0, 0, 0));
						}
						else
						{
							// ruch o pole do przodu
							Moves::addMoves(moves, setMove(square, targetSquare, 0, 0, 0, 0, 0));
							// ruch o dwa pola do przodu
							if (rank == 1 && board[square + 32] == e)
								Moves::addMoves(moves, setMove(square, square + 32, 0, 0, 1, 0, 0));
						}
					}

					// ruchy pionka - z biciem
					for (int i = 2; i < 4; i++)
					{
						int pawnOffset = bishopOffsets[i];
						int targetSquare = square + pawnOffset;

						if (!(targetSquare & 0x88))
						{
							// bicia i bicia z promocja
							if (board[targetSquare] < p)
							{
								if (rank == 6)
								{
									Moves::addMoves(moves, setMove(square, targetSquare, q, 1, 0, 0, 0));
									Moves::addMoves(moves, setMove(square, targetSquare, r, 1, 0, 0, 0));
									Moves::addMoves(moves, setMove(square, targetSquare, b, 1, 0, 0, 0));
									Moves::addMoves(moves, setMove(square, targetSquare, n, 1, 0, 0, 0));						
								}
								else
									Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 0, 0));					
							}

							// bicie w przelocie
							if (targetSquare == enpassantSquare)
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 1, 0));
						}
					}
				}

				// czarne roszada
				if (board[square] == k)
				{
					// jesli krotka roszada jest dostepna
					if (castle & kc)
					{
						if (board[f8] == e && board[g8] == e)
						{
							// spradzanie czy krol oraz f8 nie sa atakowane
							if (!isSquareAttacked(e8, white) && !isSquareAttacked(f8, white))
								Moves::addMoves(moves, setMove(e8, g8, 0, 0, 0, 0, 1));							
						}
					}
					// jesli dluga roszada jest dostepna
					if (castle & qc)
					{
						if (board[d8] == e && board[c8] == e && board[b8] == e)
						{
							// sprawdzanie czy krol oraz d8 nie sa atakowane
							if (!isSquareAttacked(e8, white) && !isSquareAttacked(d8, white))
								Moves::addMoves(moves, setMove(e8, c8, 0, 0, 0, 0, 1));							
						}
					}
				}
			}

			// ruchy skoczka (biale i czarne)
			if (!sideToMove ? board[square] == N : board[square] == n)
			{
				for (int i = 0; i < 8; i++)
				{
					int targetSquare = square + knightOffsets[i];
					int targetPiece = board[targetSquare];
					
					// sprawdzenie czy pole jest na szachownicy
					if (!(targetSquare & 0x88))
					{
						// jezeli pole docelowe jest puste lub zajete przez bierke wroga
						if (!sideToMove ? (targetPiece >= p && targetPiece <= e) : (targetPiece == e || targetPiece <= K))
						{
							// bicie
							if (targetPiece != e)
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 0, 0));								
							else
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 0, 0, 0, 0));
						}
					}
						
				}
			}

			// ruchy krola (biale i czarne)
			if (!sideToMove ? board[square] == K : board[square] == k)
			{
				for (int i = 0; i < 8; i++)
				{
					int targetSquare = square + kingOffsets[i];
					int targetPiece = board[targetSquare];

					// sprawdzenie czy pole jest na szachownicy i czy nie jest atakowane
					if (!(targetSquare & 0x88) && !isSquareAttacked(targetSquare, (sideToMove ? white : black)))
					{
						// jezeli pole docelowe jest puste lub zajete przez bierke wroga
						if (!sideToMove ? (targetPiece >= p && targetPiece <= e) : (targetPiece == e || targetPiece <= K))
						{
							// bicie
							if (targetPiece != e)
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 0, 0));
							else
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 0, 0, 0, 0));
						}
					}

				}
			}

			// ruchy gonca i hetmana
			if (!sideToMove ? (board[square] == B || board[square] == Q) : (board[square] == b || board[square] == q))
			{
				for (int i = 0; i < 4; i++)
				{
					int targetSquare = square + bishopOffsets[i];

					// iteracja po promieniu ataku
					while (!(targetSquare & 0x88))
					{
						int targetPiece = board[targetSquare];
						
						// jesli trafi na zajete pole
						if (targetPiece != e)
						{
							// jesli pole jest zajete przez wroga
							if (!sideToMove ? (targetPiece >= p && targetPiece < e) : (targetPiece < p))
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 0, 0));
							break;
						}

						Moves::addMoves(moves, setMove(square, targetSquare, 0, 0, 0, 0, 0));

						targetSquare += bishopOffsets[i];
					}
				}
			}

			// ruchy wiezy i hetmana
			if (!sideToMove ? (board[square] == R || board[square] == Q) : (board[square] == r || board[square] == q))
			{
				for (int i = 0; i < 4; i++)
				{
					int targetSquare = square + rookOffsets[i];

					// iteracja po promieniu ataku
					while (!(targetSquare & 0x88))
					{
						int targetPiece = board[targetSquare];

						// jesli trafi na zajete pole
						if (targetPiece != e)
						{
							// jesli pole jest zajete przez wroga
							if (!sideToMove ? (targetPiece >= p && targetPiece < e) : (targetPiece < p))
								Moves::addMoves(moves, setMove(square, targetSquare, 0, 1, 0, 0, 0));
							break;
						}

						Moves::addMoves(moves, setMove(square, targetSquare, 0, 0, 0, 0, 0));

						targetSquare += rookOffsets[i];
					}
				}
			}
		}
	}
}

int Moves::isSquareAttacked(int square, int side)
{
	// ataki pionka
	if (!side)
	{
		if (!((square + 17) & 0x88) && (board[square + 17] == P))
			return 1;
		if (!((square + 15) & 0x88) && (board[square + 15] == P))
			return 1;
	}
	else
	{
		if (!((square - 17) & 0x88) && (board[square - 17] == p))
			return 1;
		if (!((square - 15) & 0x88) && (board[square - 15] == p))
			return 1;
	}

	// ataki skoczka
	for (int i = 0; i < 8; i++)
	{
		int targetSquare = square + knightOffsets[i];
		int targetPiece = board[targetSquare];
		if (!(targetSquare & 0x88))
		{
			if (!side ? targetPiece == N : targetPiece == n)
				return 1;
		}
	}

	// ataki krola
	for (int i = 0; i < 8; i++)
	{
		int targetSquare = square + kingOffsets[i];
		int targetPiece = board[targetSquare];
		if (!(targetSquare & 0x88))
		{
			if (!side ? targetPiece == K : targetPiece == k)
				return 1;
		}
	}

	// ataki gonca i hetmana
	for (int i = 0; i < 4; i++)
	{
		int targetSquare = square + bishopOffsets[i];

		while (!(targetSquare & 0x88))
		{
			int targetPiece = board[targetSquare];

			if (!side ? (targetPiece == Q || targetPiece == B) : (targetPiece == q || targetPiece == b))
				return 1;

			if (targetPiece != e)
				break;

			targetSquare += bishopOffsets[i];
		}
	}

	// ataki wiezy i hetmana
	for (int i = 0; i < 4; i++)
	{
		int targetSquare = square + rookOffsets[i];

		while (!(targetSquare & 0x88))
		{
			int targetPiece = board[targetSquare];

			if (!side ? (targetPiece == Q || targetPiece == R) : (targetPiece == q || targetPiece == r))
				return 1;

			if (targetPiece != e)
				break;

			targetSquare += rookOffsets[i];
		}
	}


	return 0;
}

int Moves::makeMove(int move, int moveFlag)
{
	if (moveFlag == allMoves)
	{
		// dekodowanie ruchu
		int fromSquare = getMoveSoruce(move);
		int targetSquare = getMoveTarget(move);
		int promotedPiece = getPromotedPiece(move);
		int enpassant = getEnpassantFlag(move);
		int dpawn = getDoublePawnMoveFlag(move);
		int castling = getCastleFlag(move);

		// wykonanie ruchu
		board[targetSquare] = board[fromSquare];
		board[fromSquare] = e;

		// w przypadku promocji
		if (promotedPiece)
			board[targetSquare] = promotedPiece;

		// w przypadku bicia w przelocie
		if (enpassant)
			board[targetSquare + ((sideToMove) ? -16 : 16)] = e;

		// resetowanie pola bicia w przelocie
		enpassantSquare = no_sq;

		// w przypadku ruchu pionka o dwa pola - ustawienie pola bicia w przelocie
		if (dpawn)
			enpassantSquare = (sideToMove) ? targetSquare - 16 : targetSquare + 16;

		// w przypadku roszady
		if (castling)
		{
			// krotka roszada
			if (targetSquare - fromSquare == 2)
			{
				board[targetSquare + 1] = e;
				board[targetSquare - 1] = (sideToMove) ? r : R;
			}
			// dluga roszada
			else
			{
				board[targetSquare - 2] = e;
				board[targetSquare + 1] = (sideToMove) ? r : R;
			}
		}

		// w przypadku ruchu krola
		if (board[targetSquare] == K || board[targetSquare] == k)
			kingSquare[sideToMove] = targetSquare;

		// aktualizacja praw do roszad
		castle &= castlingRightsArr[fromSquare];
		castle &= castlingRightsArr[targetSquare];

		// zmiana strony wykonujacej ruch
		sideToMove ^= 1;

		return 1;
	}
	else
	{
		if (getCaptureFlag(move))
			makeMove(move, allMoves);
		else
			return 0;
	}
}

bool Moves::isMoveLegal(int move)
{
	// zmienne do kopii szachownicy
	int board_c[128], kingSquare_c[2];
	int side_c, enpass_c, castle_c;

	// kopiowanie szachownicy
	memcpy(board_c, board, 512);
	memcpy(kingSquare_c, kingSquare, 8);
	side_c = sideToMove;
	enpass_c = enpassantSquare;
	castle_c = castle;

	int fromSquare = getMoveSoruce(move);
	int targetSquare = getMoveTarget(move);
	int promotedPiece = getPromotedPiece(move);
	int enpassant = getEnpassantFlag(move);
	int dpawn = getDoublePawnMoveFlag(move);
	int castling = getCastleFlag(move);

	board[targetSquare] = board[fromSquare];
	board[fromSquare] = e;

	if (promotedPiece)
		board[targetSquare] = promotedPiece;

	if (enpassant)
		board[targetSquare + ((sideToMove) ? -16 : 16)] = e;

	enpassantSquare = no_sq;

	if (dpawn)
		enpassantSquare = (sideToMove) ? targetSquare - 16 : targetSquare + 16;

	if (castling)
	{
		if (targetSquare - fromSquare == 2)
		{
			board[targetSquare + 1] = e;
			board[targetSquare - 1] = (sideToMove) ? r : R;
		}
		else
		{
			board[targetSquare - 2] = e;
			board[targetSquare + 1] = (sideToMove) ? r : R;
		}
	}

	if (board[targetSquare] == K || board[targetSquare] == k)
		kingSquare[sideToMove] = targetSquare;

	castle &= castlingRightsArr[fromSquare];
	castle &= castlingRightsArr[targetSquare];

	sideToMove ^= 1;

	bool returnValue;
	if (isSquareAttacked(kingSquare[sideToMove ^ 1], sideToMove))
		returnValue = false;
	else
		returnValue = true;
	
	memcpy(board, board_c, 512);
	memcpy(kingSquare, kingSquare_c, 8);
	sideToMove = side_c;
	enpassantSquare = enpass_c;
	castle = castle_c;

	return returnValue;
}

void Moves::addMoves(moveList* moves, int move)
{
	if (isMoveLegal(move))
		moves->storedMoves[moves->counter++] = move;
}

int Moves::encodeMove(int sourceSquare, int targetSquare, int promotedPieceType)
{
	int pieceType = board[sourceSquare];

	if (pieceType == p || pieceType == P)
	{
		int promoted = 0, capture = 0, doublePawn = 0, enpass = 0;

		int diff = abs(targetSquare - sourceSquare);
		// sprawdz czy ruch to skok o 2 pola
		if (diff == 32)
			doublePawn = 1;
		// sprawdz czy nastepuje promocja
		else if (targetSquare < 8 || targetSquare >= 112)
			promoted = promotedPieceType;
		
		// sprawdz czy ruch to zbicie figury
		if (diff != 16 && diff != 32)
		{
			capture = 1;
			if (board[targetSquare] == e)
				enpass = 1;
		}

		int move = setMove(sourceSquare, targetSquare, promoted, capture, doublePawn, enpass, 0);
		return move;
	}
	else if (pieceType == k || pieceType == K)
	{
		int castl = 0, capture = 0;

		// sprawdz czy ruch to roszada
		int diff = abs(targetSquare - sourceSquare);
		if (diff == 2)
			castl = 1;
		// sprawdz czy ruch to zbicie figury
		else if (board[targetSquare] != e)
			capture = 1;

		int move = setMove(sourceSquare, targetSquare, 0, capture, 0, 0, castl);
		return move;
	}
	
	int capture = 0;
	// sprawdz czy ruch to zbicie figury przeciwnika
	if (board[targetSquare] != e)
		capture = 1;

	int move = setMove(sourceSquare, targetSquare, 0, capture, 0, 0, 0);
	return move;
}

int Moves::evaluate()
{
	int eval = 0;
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 16 + file;
			eval += piecesValues[board[square]];

			switch (board[square])
			{
			case P: eval += pawnScores[square]; break;
			case N: eval += knightScores[square]; break;
			case B: eval += bishopScores[square]; break;
			case R: eval += rookScores[square]; break;
			case K: eval += kingScores[square]; break;
			case p: eval -= pawnScores[mirrorScores[square]]; break;
			case n: eval -= knightScores[mirrorScores[square]]; break;
			case b: eval -= bishopScores[mirrorScores[square]]; break;
			case r: eval -= rookScores[mirrorScores[square]]; break;
			case k: eval -= kingScores[mirrorScores[square]]; break;
			}
		}
	}
	if (sideToMove == black)
		eval = eval * (-1);

	return eval;
}

int Moves::quiescence(int alpha, int beta)
{
	nodes++;

	int evaluation = evaluate();

	if (evaluation >= beta)
	{
		return beta;
	}

	if (evaluation > alpha)
	{
		alpha = evaluation;
	}

	moveList moves[1];
	generateMoves(moves);

	sortMoves(moves);

	for (int i = 0; i < moves->counter; i++)
	{
		int board_c[128], kingSquare_c[2];
		int side_c, enpass_c, castle_c;
		memcpy(board_c, board, 512);
		memcpy(kingSquare_c, kingSquare, 8);
		side_c = sideToMove;
		enpass_c = enpassantSquare;
		castle_c = castle;

		ply++;

		if (makeMove(moves->storedMoves[i], onlyCaptures) == 0)
		{
			ply--;
			continue;
		}

		int eval = -quiescence(-beta, -alpha);

		ply--;

		memcpy(board, board_c, 512);
		memcpy(kingSquare, kingSquare_c, 8);
		sideToMove = side_c;
		enpassantSquare = enpass_c;
		castle = castle_c;

		if (eval >= beta)
		{
			return beta;
		}

		if (eval > alpha)
		{
			alpha = eval;
		}
	}

	return alpha;
}

int Moves::negamax(int alpha, int beta, int depth)
{
	if (depth == 0)
		//return evaluate();
		return quiescence(alpha, beta);

	nodes++;

	int inCheck = isSquareAttacked(kingSquare[sideToMove], sideToMove ^ 1);

	if (inCheck) depth++;

	int bestMoveSoFar = 0;
	int oldAlpha = alpha;

	moveList moves[1];
	generateMoves(moves);

	sortMoves(moves); 

	if (moves->counter == 0)
	{
		if (inCheck)
			return -49000 + ply;
		else
			return 0;
	}

	for (int i = 0; i < moves->counter; i++)
	{
		int board_c[128], kingSquare_c[2];
		int side_c, enpass_c, castle_c;
		memcpy(board_c, board, 512);
		memcpy(kingSquare_c, kingSquare, 8);
		side_c = sideToMove;
		enpass_c = enpassantSquare;
		castle_c = castle;

		ply++;

		makeMove(moves->storedMoves[i], allMoves);

		int eval = -negamax(-beta, -alpha, depth - 1);

		ply--;

		memcpy(board, board_c, 512);
		memcpy(kingSquare, kingSquare_c, 8);
		sideToMove = side_c;
		enpassantSquare = enpass_c;
		castle = castle_c;

		if (eval >= beta)
		{
			killerMoves[1][ply] = killerMoves[0][ply];
			killerMoves[0][ply] = moves->storedMoves[i];

			return beta;
		}

		if (eval > alpha)
		{
			histMoves[board[getMoveSoruce(moves->storedMoves[i])]][getMoveTarget(moves->storedMoves[i])] += depth;

			alpha = eval;
			if (ply == 0)
				bestMoveSoFar = moves->storedMoves[i];
		}		
	}

	if (oldAlpha != alpha)
		bestMove = bestMoveSoFar;

	return alpha;
}

void Moves::searchPosition(int depth)
{
	int eval = negamax(-50000, 50000, depth);

	printf("score %d depth %d nodes %ld\n", eval, depth, nodes);
	printf("%s%s\n", squareToCoords[getMoveSoruce(bestMove)], squareToCoords[getMoveTarget(bestMove)]);
}

int Moves::scoreMove(int move)
{
	// ocena ataku
	if (getCaptureFlag(move))
	{
		int targetPiece = P;
		if (board[getMoveTarget(move)] != e)
			targetPiece = board[getMoveTarget(move)];
		// ocena poprzez mmv lva
		return mvv_lva[board[getMoveSoruce(move)]][board[getMoveTarget(move)]] + 10000;
	}
	// ocena cichego ruchu
	else
	{
		if (killerMoves[0][ply] == move)
			return 9000;
		else if (killerMoves[1][ply] == move)
			return 8000;
		else
			return histMoves[board[getMoveSoruce(move)]][getMoveTarget(move)];
	}

	return 0;
}

int Moves::sortMoves(moveList* moves)
{
	int* moveScores = new int[moves->counter];

	for (int i = 0; i < moves->counter; i++)
		moveScores[i] = scoreMove(moves->storedMoves[i]);
	
	for (int current = 0; current < moves->counter; current++)
	{
		for (int next = current + 1; next < moves->counter; next++)
		{
			if (moveScores[current] < moveScores[next])
			{
				int tempScore = moveScores[current];
				moveScores[current] = moveScores[next];
				moveScores[next] = tempScore;

				int tempMove = moves->storedMoves[current];
				moves->storedMoves[current] = moves->storedMoves[next];
				moves->storedMoves[next] = tempMove;
			}
		}
	}

	return 1;
}
#include "board.h"

void Board::resetBoard()
{
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			int square = rank * 16 + file;
			board[square] = e;
		}

		sideToMove = -1;
		enpassantSquare = no_sq;
		castle = 0;
	}
}

int Board::pieceCharToEnum(char ch)
{
	switch (ch)
	{
	case 'P': return P; break;
	case 'N': return N; break;
	case 'B': return B; break;
	case 'R': return R; break;
	case 'Q': return Q; break;
	case 'K': return K; break;
	case 'p': return p; break;
	case 'n': return n; break;
	case 'b': return b; break;
	case 'r': return r; break;
	case 'q': return q; break;
	case 'k': return k; break;
	}
	return e;
}

void Board::parseFen(char* fen)
{
	resetBoard();

	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 16; file++)
		{
			int square = rank * 16 + file;
			// dopasowywanie znak po znaku
			// bierki
			if (!(square & 0x88))
			{
				if ((*fen >= 'a' && *fen <= 'z') || (*fen >= 'A' && *fen <= 'Z'))
				{
					if (*fen == 'K')
						kingSquare[white] = square;
					else if (*fen == 'k')
						kingSquare[black] = square;

					board[square] = pieceCharToEnum(*fen);

					fen++;
				}

				// puste pola
				if (*fen >= '0' && *fen <= '9')
				{
					int offset = *fen - '0';
					if ((board[square]) == e)
						file--;

					file += offset;

					fen++;
				}

				if (*fen == '/')
					fen++;
			}
		}
	}

	// ktora strona zaczyna
	fen++;
	sideToMove = (*fen == 'w') ? white : black;

	// informacje o dozowlonych roszadach
	fen += 2;
	while (*fen != ' ')
	{
		switch (*fen)
		{
		case 'K': castle |= Kc; break;
		case 'Q': castle |= Qc; break;
		case 'k': castle |= kc; break;
		case 'q': castle |= qc; break;
		case '-': break;
		}

		fen++;
	}

	// pole enpassant
	fen++;
	if (*fen != '-')
	{
		int rank = fen[0] - 'a';
		int file = 8 - (fen[1] - '0');
		enpassantSquare = file * 16 + rank;
	}
	else
		enpassantSquare = no_sq;
}
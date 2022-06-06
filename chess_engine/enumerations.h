#ifndef ENUMERATION_H
#define ENUMERATION_H

enum { P, N, B, R, Q, K, p, n, b, r, q, k, e, o };

enum squares {
	a8 = 0, b8, c8, d8, e8, f8, g8, h8,
	a7 = 16, b7, c7, d7, e7, f7, g7, h7,
	a6 = 32, b6, c6, d6, e6, f6, g6, h6,
	a5 = 48, b5, c5, d5, e5, f5, g5, h5,
	a4 = 64, b4, c4, d4, e4, f4, g4, h4,
	a3 = 80, b3, c3, d3, e3, f3, g3, h3,
	a2 = 96, b2, c2, d2, e2, f2, g2, h2,
	a1 = 112, b1, c1, d1, e1, f1, g1, h1, no_sq
};

enum castling { Kc = 1, Qc = 2, kc = 4, qc = 8 };

enum sides { white, black };

enum captureFlags { allMoves, onlyCaptures };

extern char asciiPieces[];

extern int promotedPieces[];
extern const char* promPieces[];

extern int piecesValues[];

// pozycje FEN
extern char startPosition[];
extern char trickyPosition[];
extern char promotionPosition[];
extern char ultraPromotionPosition[];
extern char testPosition[];
extern char cmkPosition[];

extern const int knightOffsets[8];
extern const int bishopOffsets[4];
extern const int rookOffsets[4];
extern const int kingOffsets[8];

extern const char* squareToCoords[128];

extern const int castlingRightsArr[128];

extern const int pawnScores[128];
extern const int knightScores[128];
extern const int bishopScores[128];
extern const int rookScores[128];
extern const int kingScores[128];
extern const int mirrorScores[128];

extern const int mvv_lva[12][12];

extern int killerMoves[2][64];
extern int histMoves[12][128];

/*
	0000 0000 0000 0000 0111 1111	source square		0x7F
	0000 0000 0011 1111 1000 0000	target square		0x3F80
	0000 0011 1100 0000 0000 0000	promoted piece		0x3C000
	0000 0100 0000 0000 0000 0000	capture flag		0x40000
	0000 1000 0000 0000 0000 0000	double pawn flag	0x80000
	0001 0000 0000 0000 0000 0000	enpassant flag		0x100000
	0010 0000 0000 0000 0000 0000	castling flag		0x200000
*/

#define setMove(source, target, promoted, capture, doublePawn, enpass, castl) \
( (source) | (target << 7) | (promoted << 14) | (capture << 18) | (doublePawn << 19) | (enpass << 20) | (castl << 21) )

#define getMoveSoruce(move) (move & 0x7F)
#define getMoveTarget(move) ((move >> 7) & 0x7F)
#define getPromotedPiece(move) ((move >> 14) & 0xF)
#define getCaptureFlag(move) ((move >> 18) & 1)
#define getDoublePawnMoveFlag(move) ((move >> 19) & 1)
#define getEnpassantFlag(move) ((move >> 20) & 1)
#define getCastleFlag(move) ((move >> 21) & 1)

#endif // !ENUMERATION_H
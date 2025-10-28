#include <stdbool.h>
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

typedef struct {
    char* name;
    char* symbol;
    int x;
    int y;
    int is_white;
} Piece;

// Prototypen der Funktionen
void draw_chessboard(Piece* pieces);
void create_pieces(Piece* pieces);
bool introduction();
#endif

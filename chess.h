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
void draw_chessboard(Piece* pieces, bool istwhite);
void create_pieces(Piece* pieces);
char * introduction();
bool valid_move(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y);
bool is_piece(int x, int y, Piece* pieces, char* color);
#endif

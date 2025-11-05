#include <stdbool.h>
#include <string.h>
#ifndef CHESSBOARD_H
#define CHESSBOARD_H

typedef struct {
    char* name;
    char* symbol;
    int x;
    int y;
    int is_white;
    bool has_moved;
} Piece;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    char * fen;
    int depth;
    int max_thinking_time;

} API_call;

// Prototypen der Funktionen
void draw_chessboard(Piece* pieces, char* color);
void create_pieces(Piece* pieces);
char * introduction();
bool valid_move(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y, Position passant);
bool is_piece(int x, int y, Piece* pieces, char* color);
char * make_fen(Piece* pieces, char * color, int zug_counter, int halbzug_counter, Position passant);
bool is_castleling_possible(Piece king, Piece rook, Piece* pieces, char* color);
bool is_sqare_attacked(int x, int y, Piece* pieces, char* color);
void terminate_piece(Piece piece);
int which_piece(Piece* pieces, int x, int y);
bool is_move_safe(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y);
bool in_check(Piece* pieces, char* color);
char * make_json(API_call call);
#endif

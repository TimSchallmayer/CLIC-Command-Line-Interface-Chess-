#include <stdbool.h>
#include <string.h>
#include <cjson/cJSON.h>
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
typedef struct {
    char * data;
    size_t length;
} Buffer;

typedef struct {
    bool response;
    bool is_promotion;
    char promotion_choice;
    int mate;
    bool is_castling;
    char * text;
    char* turn;
} API_response;

// funktionen
void draw_chessboard(Piece* pieces, char* color);
void create_pieces(Piece* pieces);
char * introduction(int* difficulty);
bool valid_move(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y);
bool is_piece(int x, int y, Piece* pieces, char* color);
char * make_fen(Piece* pieces, char * color, int zug_counter, int halbzug_counter);
bool is_castleling_possible(Piece king, Piece rook, Piece* pieces, char* color);
bool is_sqare_attacked(int x, int y, Piece* pieces, char* color);
void terminate_piece(Piece *piece);
int which_piece(Piece* pieces, int x, int y);
bool is_move_safe(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y);
bool in_check(Piece* pieces, char* color);
char * make_json(API_call call);
char * curl(char* json);
int callback(void *content, size_t size, size_t nmemb, void *user_pointer);
API_response api_move(cJSON* response_json, Piece* pieces, char* play_color, int* zug_counter, int* halbzug_counter, Piece* terminated_pieces);
void make_move(Piece* pieces, Piece* terminated_pieces, char* play_color, int origin_x, int origin_y, int position_x, int position_y, int* zug_counter, int* halbzug_counter, char * api_color, API_response response);
void check_game_over(char* play_color, API_response respsonse, Piece* pieces);
int is_mate(Piece * pieces, char* user_color);
#endif

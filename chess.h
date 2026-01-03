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
// struct für die einzelnen Figuren.

typedef struct {
    int x;
    int y;
} Position;
// struct für eine Position auf dem Schachbrett. (Wird jetzt nicht mehr verwendet da es nur für en passant nützlich war) 
typedef struct {
    char * fen;
    int depth;
    int max_thinking_time;

} API_call;
// struct für den Api call bei dem die einzelnen Parameter für die Api übergeben werden. (wird später in json umgewandelt)


typedef struct {
    char * data;
    size_t length;
} Buffer;
// struct für einen speicher block, welcher in der callbackfunktion von libcurl benutzt wird.

typedef struct {
    bool response;
    bool is_promotion;
    char promotion_choice;
    int mate;
    bool is_castling;
    char * text;
    char* turn;
} API_response;
//struct für die API Antwort. (wurde aus den Information der json Antwort der Api erstellt)


// funktionen
void draw_chessboard(Piece* pieces, char* color);
// "Zeichnet" das Schachbrett in die Befehlszeile, wobei sich die Ausrichtung an der Spielerfarbe orientiert. Das bedeutet die Spielerfarbe ist immer unten.

void create_pieces(Piece* pieces);
// Erstellt die einzelnen Schachfiguren mit ihren Simbolen an ihren Ursprungspositionen.

char * introduction(int* difficulty);
// Fragt anfangs Parameter wie Schwierigkeit und Spielerfarbe ab und erklärt bei bedarg die Regeln.

bool valid_move(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y);
// Überprüft ob ein Zug einer Figur legal ist.
// --> Gibt true zurück falls ja und false falls nein.

bool is_piece(int x, int y, Piece* pieces, char* color);
// Überprüft ob sich an den angegebenen Koordinaten eine Firgur befindet. Man dies bei Bedarf auch auf die Farbe spezialisieren. Ansonsten benutzt man einfach nur "both".
// --> Gibt true zurück falls ja und false falls nein.

char * make_fen(Piece* pieces, char * color, int zug_counter, int halbzug_counter);
// Erstellt die FEN, welche die Api braucht um den Aktuellen Spielstand zu erfahren. Gibt die FEN als char * zurück.
// Mehr zu der FEN findet man hier: https://www.chess.com/de/terms/forsyth-edwards-notation-fen

bool is_castleling_possible(Piece king, Piece rook, Piece* pieces, char* color);
// Prüft ob eine Rochade möglich ist. Der Parameter color gibt Farbe des Spielers an.
// --> Gibt true zurück falls ja und false falls nein.

bool is_sqare_attacked(int x, int y, Piece* pieces, char* color);
// Prüft ob die angegebene Koordinate von gegenerischen Figuren bedroht wird.
// --> Gibt true zurück falls ja und false falls nein.

void terminate_piece(Piece *piece);
// Setzt eine Figur auf die Position (-1;-1), wodurch sie nicht mehr am Spiel teilnehmen kann.
// --> valid move gibt bei der Ursprungsposition (-1;-1) immer false zurück.

int which_piece(Piece* pieces, int x, int y);
// Gibt einen Index zurück, welche die Position, der Figur an der gewählten Koordinaten, 
// in der Figurenliste beschreibt.

bool is_move_safe(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y);
// Prüft ob ein Zug sicher für eine Figure wäre. 
// --> (ob der Gegner sie nach dem Zug schlagen kann, wird hauptsächlich für den König verwendet)
// --> Gibt true zurück falls ja und false falls nein.

bool in_check(Piece* pieces, char* color);
//Prüft ob der König einer Spielfarbe im Schach steht.
// --> Gibt true zurück falls ja und false falls nein.

char * make_json(API_call call);
// Erstellt das Json auf der Basis des Api calls, welches an die Api übergeben wird.
// Das Json wird als char * zurückgegeben.

char * curl(char* json);
// Führt die API Anfrage durch.

int callback(void *content, size_t size, size_t nmemb, void *user_pointer);
// Speichert die Antwort der API.
// --> Wird nicht manuell aufgerufen sondern wird von der Bibiliothek libcurl verwendet.

API_response api_move(cJSON* response_json, Piece* pieces, char* play_color, int* zug_counter, int* halbzug_counter, Piece* terminated_pieces);
// Führt den Zug der Api aus.

void make_move(Piece* pieces, Piece* terminated_pieces, char* play_color, int origin_x, int origin_y, int position_x, int position_y, int* zug_counter, int* halbzug_counter, char * api_color, API_response response);
// führt einen Zug aus. 
// --> Wird in api_move aufgerufen und sorgt somit nicht für den Zug des Nutzers sodnern auch den der API aus.

void check_game_over(char* play_color, API_response respsonse, Piece* pieces);
// Prüft ob das Spiel zu ende ist.

int is_mate(Piece * pieces, char* user_color);
// Prüft ob eine Spielpartei Schachmatt, Patt oder garnichts ist.
// Der Rückgabewert 0 bedeutet das eine Spielpartei keine legalen Züge mehr hat 
// und so kann nach weiterer Überprüfung durch in_check() geprüft werden ob es ein Matt oder Patt ist.

#endif

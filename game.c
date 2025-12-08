#include <stdio.h>
#include "chess.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <cjson/cJSON_Utils.h>
#include <curl/curl.h>


void draw_chessboard(Piece* pieces, char* color) {
    printf("  +----+----+----+----+----+----+----+----+\n");
    if (strcmp(color, "black") == 0) {
        for (int i = 0; i < 8; i++) {
            printf("%i |", i + 1);
            for (int j = 0; j < 8; j++) {
                bool printed = false;
                for (int k = 0; k < 32; k++) {
                    if (pieces[k].x == j && pieces[k].y == i) {
                        printf(" %s  |", pieces[k].symbol);
                        printed = true;
                        break;
                    }
                }
                if (!printed) {
                    printf("    |");
                }
            }
            printf("\n");
            printf("  +----+----+----+----+----+----+----+----+\n");
        }
    }
    else {
        for (int i = 7; i >= 0; i--) {
            printf("%i |", i + 1 );
            for (int j = 0; j < 8; j++) {
                bool printed = false;
                for (int k = 0; k < 32; k++) {
                    if (pieces[k].x == j && pieces[k].y == i) {
                        printf(" %s  |", pieces[k].symbol);
                        printed = true;
                        break;
                    }
                }
                if (!printed) {
                    printf("    |");
                }
            }
            printf("\n");
            printf("  +----+----+----+----+----+----+----+----+\n");
        }
    }
    printf("    Ａ   Ｂ   Ｃ   Ｄ   Ｅ   Ｆ   Ｇ   Ｈ\n");

}
void create_pieces(Piece* pieces) {

  //  printf("Creating pieces...\n");
    // black pieces
    pieces[0] = (Piece){"Rook", "♖", 0, 7, true, false};
    pieces[1] = (Piece){"Knight", "♘", 1, 7, true, false};
    pieces[2] = (Piece){"Bishop", "♗", 2, 7, true, false};
    pieces[3] = (Piece){"Queen", "♕", 3, 7, true, false};
    pieces[4] = (Piece){"King", "♔", 4, 7, true, false};
    pieces[5] = (Piece){"Bishop", "♗", 5, 7, true, false};
    pieces[6] = (Piece){"Knight", "♘", 6, 7, true, false};
    pieces[7] = (Piece){"Rook", "♖", 7, 7, true, false};
    for (int i = 0; i < 8; i++) {
        pieces[8 + i].name = malloc(5 * sizeof(char));
        pieces[8 + i].symbol = malloc(4 * sizeof(char));
        strcpy(pieces[8 + i].name, "Pawn");
        strcpy(pieces[8 + i].symbol, "♙"); 
  
        pieces[8 + i].x = i;
        pieces[8 + i].y = 6;
        pieces[8 + i].is_white = true;
        pieces[8 + i].has_moved = false;
    }

    // white pieces (dumme logik mit den iswhite)
    pieces[16] = (Piece){"Rook", "♜", 0, 0, false, false};
    pieces[17] = (Piece){"Knight", "♞", 1, 0, false, false};
    pieces[18] = (Piece){"Bishop", "♝", 2, 0, false, false};
    pieces[19] = (Piece){"Queen", "♛", 3, 0, false, false};
    pieces[20] = (Piece){"King", "♚", 4, 0, false, false};
    pieces[21] = (Piece){"Bishop", "♝", 5, 0, false, false};
    pieces[22] = (Piece){"Knight", "♞", 6, 0, false, false};
    pieces[23] = (Piece){"Rook", "♜", 7, 0, false, false};
    for (int i = 0; i < 8; i++) {
        
        pieces[24 + i].name = malloc(5 * sizeof(char));
        pieces[24 + i].symbol = malloc(4 * sizeof(char));
        strcpy(pieces[24 + i].name, "Pawn");
        strcpy(pieces[24 + i].symbol, "♟"); 
        pieces[24 + i].x = i;
        pieces[24 + i].y = 1;
        pieces[24 + i].is_white = false;
        pieces[24 + i].has_moved = false;

      
    }
}
char * introduction(int * difficulty) {
    char choice;
    printf("Welcome to Command-Line Chess!\nFor the game instructions, please type 'help'.\n");
    bool valid_input = true;
    while (valid_input) {
        printf("Please enter your desired difficulty level (1-18) or press Enter to keep the default (12): ");
        
        char line[16];
        fgets(line, sizeof(line), stdin); 
        
        if (line[0] == '\n') {
            *difficulty = 12; 
            valid_input = false;
            break;
        }

        int diff = atoi(line); 
        if (diff >= 1 && diff <= 18) {
            *difficulty = diff;
            valid_input = false;
        } else {
            printf("Invalid input. Try again.\n");
        }
}
    printf("Would you like to play as white, black or random? ");
    char* color = malloc(6);
    while (true) {
        scanf("%5s", color);
        for (int i = 0; color[i]; i++) {
            color[i] = tolower(color[i]);
        }
        printf("\n");
        if (strcmp(color, "white") == 0 || color[0] == 'w') {
            printf("                 Playing white.\n");
            strcpy(color, "white");
            return color;

        } else if (strcmp(color, "black") == 0 || color[0] == 'b') {
            printf("                 Playing black.\n");
            strcpy(color, "black");
            return color;
        } else if (strcmp(color, "random") == 0 || color[0] =='r')
        {
            srand(time(NULL)); 
            bool is_white = rand() % 2;
            if (is_white) { 
                printf("                 Playing white.\n");
                strcpy(color, "white");
            }
            else {
                printf("                 Playing black.\n");
                strcpy(color, "black");
            } 
            return color;
        }
        
        
        else if (strcmp(color,"help") == 0 ||  color[0] == 'h') {
            printf("GAME INTRODUCTIONS: \n");
            printf("1. You play as the color you choose (white, black or random) or respectively (w, b, r).\n");
            printf("2. You can move a piece by stating the name, origin and position to where it schould go. For Example: Rook A8 A5.\nYou can also use the fist Letter of each piece (R A8 A5). The coordinates and pieces are not case sensitive.\n");
            printf("3. Due to the limitations of the used API, some special moves like en passant are not supported.\n");
            printf("4. Type ctr + c to exit the pogramm.\n");
            printf("5. Make shure to have a utf-8 terminal\n");
            printf("6. Now pick a color: ");
            continue;
        }
        else{
            printf("Invalid choice. Please type 'white' or 'black': ");
            continue;
        }
    }

    
}

bool valid_move(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y) {
    bool is_white = !piece.is_white;
    char * color;
    char * opponent_color;
    if (is_white) {
        color = "white";
        opponent_color = "black";
    }
    else {
        color = "black";
        opponent_color = "white";
    }

    
    if (origin_x == -1 || origin_y == -1) {
        return false;

    }

    if (is_piece(position_x, position_y, pieces, color)) {
        return false;
    }
    if (piece.x != origin_x || piece.y != origin_y) {
        return false;
    }
    else if (strcmp(piece.name, "Pawn") == 0) {
        if (strcmp(color, "white") == 0)
        {
            if (position_y == origin_y + 1 && position_x == origin_x && !is_piece(position_x, position_y, pieces, "both")) {
                return true;
            }
            else if (position_x == origin_x && position_y == origin_y + 2 && piece.y == 1 && !is_piece(position_x, position_y, pieces, "both"))
            {
                return true;
            }
            
            else if ((position_x == origin_x + 1 || position_x == origin_x -1) && position_y == origin_y + 1) {
                if (is_piece(position_x, position_y, pieces, opponent_color))
                {
                    return true;
                }
                else {
                    return false;
                }
            }
            
            else { 
                return false;
            }    /* code */
        }
        else if (strcmp(color, "black") == 0)
        {
            if (position_y == origin_y - 1 && position_x == origin_x && !is_piece(position_x, position_y, pieces, "both")) {
                return true;
            }
            else if (position_x == origin_x && position_y == origin_y - 2 && piece.y == 6 && !is_piece(position_x, position_y, pieces, "both"))
            {
                return true;
            }
            else if ((position_x == origin_x + 1 || position_x == origin_x -1) && position_y == origin_y - 1) {
                if (is_piece(position_x, position_y, pieces, opponent_color))
                {
                    return true;
                }
                else {
                    return false;
                }
            }
            else {
                return false;
            }
        }
        
        
    }
    else if (strcmp(piece.name, "Rook") == 0) {

        if (position_x == origin_x) {
            if (origin_y > position_y) {
               for (int i = origin_y - 1; i > position_y; i--) {
                    if(is_piece(origin_x, i, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
            else if (origin_y < position_y) {
               for (int i = origin_y + 1; i < position_y; i++) {
                    if(is_piece(origin_x, i, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
            else {
                return true;
            }
        }
        else if (position_y == origin_y) {
            if (origin_x > position_x) {
               for (int i = origin_x - 1; i > position_x; i--) {
                    if(is_piece(i, origin_y, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
            else if (origin_x < position_x) {
               for (int i = origin_x + 1; i < position_x; i++) {
                    if(is_piece(i, origin_y, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
            else {
                return true;
            }
        }
        else {
            return false;
        }
    }
    else if (strcmp(piece.name , "Knight") == 0)
    {
        if (((position_y == origin_y + 2 || position_y == origin_y - 2) && (position_x == origin_x +1 || position_x == origin_x -1)) || ((position_x == origin_x + 2 || position_x == origin_x - 2) && (position_y == origin_y +1 || position_y == origin_y -1)))
        {
            return true;
        }
        else {
            return false;
        }
        
    }
    else if (strcmp(piece.name, "Bishop") == 0)
    {
        int diff_x = 0;
        int diff_y = 0;
        int x = origin_x;
        int y = origin_y;
        int step_x;
        int step_y;

        if (origin_x > position_x) {
            diff_x = origin_x - position_x;
            step_x = -1;
        }
        else if (position_x > origin_x)
        {
            diff_x = position_x - origin_x;
            step_x = 1;
        }
        if (origin_y > position_y) {
            diff_y = origin_y - position_y;
            step_y = -1;
        }
        else if (position_y > origin_y)
        {
            diff_y = position_y - origin_y;
            step_y = 1;
        }

        if (diff_x != diff_y) {
            return false;
        }

        x += step_x;
        y += step_y;  

        while (x != position_x && y != position_y) 
        {
            if (is_piece(x , y, pieces, "both")) {
                return false;
            }
            x += step_x;
            y += step_y;
        }
        return true;
        
    }
    else if (strcmp(piece.name, "Queen") == 0)
    {
        int diff_x;
        int diff_y;
        int x = origin_x;
        int y = origin_y;
        int step_x;
        int step_y;

        if (position_x == origin_x) {
            if (origin_y > position_y) {
               for (int i = origin_y - 1; i > position_y; i--) {
                    if(is_piece(origin_x, i, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
            else if (origin_y < position_y) {
               for (int i = origin_y + 1; i < position_y; i++) {
                    if(is_piece(origin_x, i, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
        }
        else if (position_y == origin_y) {
            if (origin_x > position_x) {
               for (int i = origin_x -1; i > position_x; i--) {
                    if(is_piece(i, origin_y, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
            else if (origin_x < position_x) {
               for (int i = origin_x +1; i < position_x; i++) {
                    if(is_piece(i, origin_y, pieces, "both")) {
                        return false;
                    }
               }
               return true;
            }
        }
        else {

            if (origin_x > position_x) {
                diff_x = origin_x - position_x;
                step_x = -1;
            }
            else if (position_x > origin_x)
            {
                diff_x = position_x - origin_x;
                step_x = 1;
            }

            if (origin_y > position_y) {
                diff_y = origin_y - position_y;
                step_y = -1;
            }
            else if (position_y > origin_y)
            {
                diff_y = position_y - origin_y;
                step_y = 1;
            }

            if (diff_x != diff_y) {
                return false;
            }

            x += step_x;
            y += step_y;  
            
            while (x != position_x && y != position_y) 
            {
                if (is_piece(x , y, pieces, "both")) {
                    return false;
                }
                x += step_x;
                y += step_y;
            }
            return true;
        }
    }
    else if (strcmp(piece.name, "King") == 0)
    {
        if (abs(origin_x - position_x) == 2 )
        {
            //printf("Castleling move detected.\n");
            if (is_castleling_possible(piece, pieces[which_piece(pieces, origin_x - 4, piece.y)], pieces, color) && origin_x > position_x)
            {
                pieces[which_piece(pieces, origin_x - 4, piece.y)].x = origin_x -1;
                return true;
            }
        
            if (is_castleling_possible(piece, pieces[which_piece(pieces, origin_x + 3, piece.y)], pieces, color) && position_x > origin_x)
            {
                pieces[which_piece(pieces, origin_x + 3, piece.y)].x = origin_x +1;
                return true;
            }
            
        }
        
        
        if (abs(position_x - origin_x) > 1 || abs(position_y - origin_y) > 1) {
            return false;
        }
        return true;
    }
    return false;
}

bool is_piece(int x, int y, Piece* pieces, char * color) {

    for (int i = 0; i < 32; i++) {
        if (pieces[i].x == x && pieces[i].y == y) {
            if (strcmp(color, "black") == 0 && pieces[i].is_white)
            {
                return true;        
            }
            else if (strcmp(color, "white") == 0 && !pieces[i].is_white)
            {
                return true;
            }
            else if (strcmp(color, "both") == 0)
            {
                return true;
            }
            
        
        }
        else {
            continue;
        }
    }
        return false;

// dummer code (like warum, was hab ich mir dabei gedacht)
/*
    if (is_white) {
        for(int i = 16; i < 32; i++) {
            if (pieces[i].x == x && pieces[i].y == y && is_white) {
                break;
                return true;
            }
            else {
                continue;
            }
        }
        return false;
    }
    else {
        for(int i = 0; i < 16; i++) {
            if (pieces[i].x == x && pieces[i].y == y && is_white) {
                break;
                return true;
            }
            else {
                continue;
            }
        }
        return false;
    }*/
}

char * make_fen(Piece* pieces, char * color, int zug_counter, int halbzug_counter) {
    char * fen = calloc(200, sizeof(char));
    if (!fen) return NULL;

    int passed = 0;
    bool found = false;

    for (int i = 7; i >= 0; i--) { 
        passed = 0;

        for (int j = 0; j < 8; j++) {
            found = false;

            for (int k = 0; k < 32; k++) {
                if (pieces[k].x == j && pieces[k].y == i) {

                    if (passed > 0) {
                        char passed_str[2] = {passed + '0', '\0'};
                        strcat(fen, passed_str);
                        passed = 0;
                    }

            
                    char symbol;
                    if (strcmp(pieces[k].name, "Knight") == 0)
                        symbol = 'N';
                    else
                        symbol = pieces[k].name[0];

                    if (pieces[k].is_white)
                        symbol = tolower(symbol);

                    char symbol_str[2] = {symbol, '\0'};
                    strcat(fen, symbol_str);

                    found = true;
                    break; 
                }
            }

            if (!found) {
                passed++;
            }
        }

        if (passed > 0) {
            char passed_str[2] = {passed + '0', '\0'};
            strcat(fen, passed_str);
        }

        if (i > 0) strcat(fen, "/");
    }

    if (strcmp(color, "white") == 0) strcat(fen, " b");
    else strcat(fen, " w");
    bool whiteK = is_castleling_possible(pieces[20], pieces[23], pieces, "white"); 
    bool whiteQ = is_castleling_possible(pieces[20], pieces[16], pieces, "white");
    bool blackK = is_castleling_possible(pieces[4], pieces[7], pieces, "black"); 
    bool blackQ = is_castleling_possible(pieces[4], pieces[0], pieces, "black");   
    if (!whiteK && !whiteQ && !blackK && !blackQ) {
        strcat(fen, " -");
    } else {
        strcat(fen, " ");
        if (whiteK) strcat(fen, "K");
        if (whiteQ) strcat(fen, "Q");
        if (!whiteK && !whiteQ && blackK && blackQ) strcat(fen, "-");
        if (blackK) strcat(fen, "k");
        if (blackQ) strcat(fen, "q");
        if (!blackK && !blackQ && whiteK && whiteQ) strcat(fen, "-");
    }
    strcat(fen, " ");
    strcat(fen, "-");
    
    strcat(fen, " ");
    char halbzug_str[10];
    sprintf(halbzug_str, "%d", halbzug_counter);
    strcat(fen, halbzug_str);

    strcat(fen, " ");

    char zug_str[10];
    sprintf(zug_str, "%d", zug_counter);
    strcat(fen, zug_str);
    //printf("FEN: %s\n", fen);
    return fen;
}


bool is_sqare_attacked(int x, int y, Piece* pieces, char* color) {
    for (int i = 0; i < 32; i++) {
        if ((pieces[i].is_white && strcmp(color, "black") == 0) || (!pieces[i].is_white && strcmp(color, "white") == 0)) {
            if (strcmp(pieces[i].name, "Pawn") == 0)
            {
                if (abs(pieces[i].x - x) == 1 && (pieces[i].y - y == -1 && strcmp(color, "black") == 0) ||(pieces[i].y - y == 1 && strcmp(color, "black") == 0)) {
                    return true;
                } else {
                    continue;
                }
               // printf("2");
            }
            
            if (valid_move(pieces[i], pieces, pieces[i].x, pieces[i].y, x, y)) {
                return true;
            }
        }
    }
    return false;
}

bool is_castleling_possible(Piece king, Piece rook, Piece* pieces, char* color) {
    if (king.is_white != rook.is_white) {
        return false;
    }
    if (strcmp(color, "white") == 0) {
        
        if (king.y != 0 || rook.y != 0){
        //    printf("king.y: %d, rook.y: %d\n", king.y, rook.y);
       //     printf("castleling false due to y pos\n");
            return false;
        } 
    }
    else if (strcmp(color, "black") == 0) {
        if (king.y != 7 || rook.y != 7) {
          //  printf("castleling false due to y pos\n");
          //  printf("king.y: %d, rook.y: %d\n", king.y, rook.y);
            return false;
        }
    }
    if (is_sqare_attacked(king.x, king.y, pieces, strcmp(color, "white") == 0 ? "black" : "white")) 
    {
     //   printf("castleling false due to check\n");
        return false;
    }
    if (king.has_moved || rook.has_moved)
    {
        //printf("castleling false due to moved piece\n");
        return false;
    }
    
    int step = 0;
    if (rook.x > king.x) {
        step = 1;
    }
    else if (rook.x < king.x) {
        step = -1;
    }
   // printf("forloop");
    for (int x = king.x + step; x != rook.x; x += step) {
        if (is_piece(x, king.y, pieces, "both")) {
          //  printf("castleling false due to piece in between\n");
            return false;
        }
        if (is_sqare_attacked(x, king.y, pieces, strcmp(color, "white") == 0 ? "black" : "white")) {
         //   printf("castleling false due to attacked square inbetween\n");
            return false;
        }
    }
    return true;
}
void terminate_piece(Piece *piece) {
    piece->x = -1;
    piece->y = -1;
}

int which_piece(Piece* pieces, int x, int y) {
    for (int i = 0; i < 32; i++) {
        if (pieces[i].x == x && pieces[i].y == y) {
            return i;
        }
    }
    return -1;
}

bool in_check(Piece* pieces, char* color) {
    Piece king;
    for (int i = 0; i < 32; i++) {
        if (strcmp(pieces[i].name, "King") == 0) {
            if (!pieces[i].is_white && strcmp(color, "white") == 0 || pieces[i].is_white && strcmp(color, "black") == 0) {
                king = pieces[i];
                break;
            }
        }
    }
    return is_sqare_attacked(king.x, king.y, pieces, king.is_white ? "white" : "black");
}
bool is_move_safe(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y) {

    int i = which_piece(pieces, origin_x, origin_y);
    int opp_i = which_piece(pieces, position_x, position_y);
    int original_opp_x;
    int original_opp_y;
    if (opp_i != -1) {
        original_opp_x = pieces[opp_i].x;
        original_opp_y = pieces[opp_i].y;
        pieces[opp_i].x = -1;
        pieces[opp_i].y = -1;

    }
    int original_x = pieces[i].x;
    int original_y = pieces[i].y;
    pieces[i].x = position_x;
    pieces[i].y = position_y;

    bool safe = !in_check(pieces, piece.is_white ? "black" : "white");


    pieces[i].x = original_x;
    pieces[i].y = original_y;

    if (opp_i != -1) {
        pieces[opp_i].x = original_opp_x;
        pieces[opp_i].y = original_opp_y;
    }

    return safe;
}

char * make_json(API_call call) {

    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "fen", call.fen);
    cJSON_AddNumberToObject(json, "depth", call.depth);
    cJSON_AddNumberToObject(json, "max_thinking_time", call.max_thinking_time);
    char * json_str = cJSON_Print(json);
    cJSON_Delete(json);
    return json_str;
}
char * curl(char* json) {

    Buffer buff = {0};
    CURL *curl;
    curl = curl_easy_init();
    if (curl)
    {
        CURLcode res;
        curl_easy_setopt(curl, CURLOPT_URL, "https://chess-api.com/v1");
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
        struct curl_slist *header = NULL;
        header = curl_slist_append(header, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buff);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
       // printf("Response from API: %s\n", buff.data);
        char * response_str = buff.data;
        curl_slist_free_all(header);
        curl_easy_cleanup(curl);
        return response_str;
    }
    return ".-- IGNORE ---\n";
    
}
int callback(void *content, size_t size, size_t nmemb, void *user_pointer) {
    size_t huuge = size * nmemb;
    Buffer *mem_ptr = (Buffer *)user_pointer;
    
    char *pointer = realloc(mem_ptr->data, mem_ptr->length + huuge + 1);
    if(!pointer) return 0;

    mem_ptr->data = pointer;
    memcpy(&(mem_ptr->data[mem_ptr->length]), content, huuge);
    mem_ptr->length += huuge;
    mem_ptr->data[mem_ptr->length] = '\0';

    return huuge;
}
void make_move(Piece* pieces, Piece* terminated_pieces, char* play_color, int origin_x, int origin_y, int position_x, int position_y, int* zug_counter, int* halbzug_counter, char * api_color, API_response respsonse) 
{   
  //  printf("Processing player move...\n");

    int index_piece = which_piece(pieces, origin_x, origin_y);
    Piece choosen_piece;
    bool found = false;
    for (int i = 0; i < 32; i++) {
        if (pieces[i].x == origin_x && pieces[i].y == origin_y && ((pieces[i].is_white == false && (strcmp(play_color, "white") == 0 || strcmp(api_color, "white") == 0)) ||
            (pieces[i].is_white == true && (strcmp(play_color, "black") == 0 || strcmp(api_color, "black") == 0)))) 
        {
            choosen_piece = pieces[i];
            found = true;
            break;
        }
    }
    if (api_color != NULL)
    {
        if (is_piece(position_x, position_y, pieces, "both")) 
        {
            int index = -1;
            index = which_piece(pieces, position_x, position_y);
            if (index != -1) {
                printf("%s captures %s\n", choosen_piece.name, pieces[index].name);
                terminate_piece(&pieces[index]);
            }
        }
    }
    char* position = malloc(3);
    position[0] = position_x + 'a';
    position[1] = position_y + '1';
    position[2] = '\0';

   // printf("  Moving %s to %s\n", pieces[i].name, position);

    if (strcmp(pieces[index_piece].name, "Pawn") == 0) {

        if (position_y == (pieces[index_piece].is_white ? 0 : 7)) {  
            printf("Pawn promotion available!\n");
            bool invalid = true;
            while (invalid && !respsonse.response) {
                printf("Pawn promoted to: ");
                char promotion_choice[7];
                scanf("%6s", promotion_choice);
                promotion_choice[0] = toupper(promotion_choice[0]);
                invalid = false;
                if (strcmp(promotion_choice, "Knight") != 0 && strcmp(promotion_choice, "Bishop") != 0 && strcmp(promotion_choice, "Rook") != 0 && strcmp(promotion_choice, "Queen") != 0) {
                    printf("Invalid promotion choice!\n");
                    invalid = true;
                    continue;
                }
                if (strcmp(promotion_choice, "Knight") == 0)
                    pieces[index_piece].symbol = pieces[index_piece].is_white ? "♘" : "♞";
                else if (strcmp(promotion_choice, "Bishop") == 0)
                    pieces[index_piece].symbol = pieces[index_piece].is_white ? "♗" : "♝";
                else if (strcmp(promotion_choice, "Rook") == 0)
                    pieces[index_piece].symbol = pieces[index_piece].is_white ? "♖" : "♜";
                else if (strcmp(promotion_choice, "Queen") == 0)
                    pieces[index_piece].symbol = pieces[index_piece].is_white ? "♕" : "♛";
                strcpy(pieces[index_piece].name, promotion_choice);                
            }
            if (respsonse.is_promotion && respsonse.response) {      
                char promotion = respsonse.promotion_choice;
                if (promotion == 'n') {
                    strcpy(pieces[index_piece].symbol, pieces[index_piece].is_white ? "♘" : "♞");
                    strcpy(pieces[index_piece].name, "Knight");
                    }
                else if (promotion == 'b')
                {
                    strcpy(pieces[index_piece].symbol, pieces[index_piece].is_white ? "♗" : "♝");
                    strcpy(pieces[index_piece].name, "Bishop");
                }
                else if (promotion == 'r') {
                    strcpy(pieces[index_piece].symbol, pieces[index_piece].is_white ? "♖" : "♜");
                    strcpy(pieces[index_piece].name, "Rook");
                }
                else if (promotion == 'q') {
                    strcpy(pieces[index_piece].name, "Queen");
                    strcpy(pieces[index_piece].symbol, pieces[index_piece].is_white ? "♕" : "♛");
                }

            }
    
    }
}
    pieces[index_piece].x = position_x;
    pieces[index_piece].y = position_y;
    pieces[index_piece].has_moved = true;

 //   printf("Move completed.\n");
    draw_chessboard(pieces, play_color);
   // printf("Board after move:\n");
    if (strcmp(pieces[index_piece].name, "Pawn") == 0)
    {
        (*halbzug_counter) = 0;
    }
    else {
        (*halbzug_counter)++;
    }
    
/*
    if (!(abs(position_y - origin_y) == 2 && strcmp(pieces[i].name, "Pawn") == 0)) {
        passant->x = -1;
        passant->y = -1;
    }*/
    check_game_over(play_color, respsonse, pieces);
    

    return;
}
void check_game_over(char* play_color, API_response respsonse, Piece* pieces)
{
    char* turnColor = (strcmp(respsonse.turn, "b") == 0) ? "black" : "white";
 //   printf("Turn color: %s\n", turnColor);
   // printf("Play color: %s\n", play_color);
    if (((respsonse.mate == 1 || respsonse.mate == -1) && strcmp(turnColor, play_color) == 0))
    {
        bool playerLost = (strcmp(turnColor, play_color) == 0);

        printf("\n           ===== GAME OVER =====\n");
        printf("           Winner: %s\n\n", playerLost ? "Computer" : "Player");
        exit(0);
    }

    else if (is_mate(pieces, play_color) == 0 || is_mate(pieces, strcmp(play_color, "white") == 0 ? "black" : "white") == 0)
    {
        printf("\n           ===== GAME OVER =====\n");
        if (in_check(pieces, play_color) || in_check(pieces, strcmp(play_color, "white") == 0 ? "black" : "white"))
        {
            printf("           Winner: %s\n\n", (in_check(pieces, play_color)) ? "Computer" : "Player");
        }
        else {
            printf("           Draw by stalemate!\n\n");
        }
        exit(0);
    }
    return;
}
int is_mate(Piece * pieces, char* user_color) {
    int count_mate = 0;
        for (int i = 0; i < 32; i++) {
            if ((pieces[i].is_white && strcmp(user_color, "black") == 0) || (!pieces[i].is_white && strcmp(user_color, "white") == 0)) {
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (valid_move(pieces[i], pieces, pieces[i].x, pieces[i].y, x, y) && is_move_safe(pieces[i], pieces, pieces[i].x, pieces[i].y, x, y)) {
                            count_mate++;
                            break; 
                        }
                    }
                }
            }
        }
    return count_mate; 
}
API_response api_move(cJSON* response_json, Piece* pieces, char* play_color, int* zug_counter, int* halbzug_counter, Piece* terminated_pieces) {

    //printf("Processing API move...\n");
    cJSON* piece = cJSON_GetObjectItemCaseSensitive(response_json, "piece");
    cJSON* from = cJSON_GetObjectItemCaseSensitive(response_json, "from");
    cJSON* to = cJSON_GetObjectItemCaseSensitive(response_json, "to");
    cJSON* promotion = cJSON_GetObjectItemCaseSensitive(response_json, "promotion");
    cJSON* is_promotion = cJSON_GetObjectItemCaseSensitive(response_json, "isPromotion");
    cJSON* is_castle = cJSON_GetObjectItemCaseSensitive(response_json, "isCastling");
    cJSON* mate_sqs = cJSON_GetObjectItemCaseSensitive(response_json, "mate");
    cJSON* msg = cJSON_GetObjectItemCaseSensitive(response_json, "text");
    cJSON* col = cJSON_GetObjectItemCaseSensitive(response_json, "turn");

   // printf("API move received: %s from %s to %s\n", piece->valuestring, from->valuestring, to->valuestring);

    int origin_x = tolower(from->valuestring[0]) - 'a';
    int origin_y = from->valuestring[1] - '1';
    int position_x = tolower(to->valuestring[0]) - 'a';
    int position_y = to->valuestring[1] - '1';
    bool ispromotion = cJSON_IsTrue(is_promotion);
    char promotion_choice = 'f';

    if (ispromotion)
    {
        promotion_choice = promotion->valuestring[0];
    }
    
    bool iscastling = cJSON_IsTrue(is_castle);
    int mate = 1000;
    if (!cJSON_IsNull(mate_sqs)) {
        if (cJSON_IsString(mate_sqs)) {
            mate = atoi(mate_sqs->valuestring);
        }
        else if (cJSON_IsNumber(mate_sqs)) {
            mate = mate_sqs->valueint;
        }
    }
    char * text = msg->valuestring;
    API_response api_response;
    api_response.is_promotion = ispromotion;
    api_response.mate = mate;
    api_response.is_castling = iscastling;
    api_response.promotion_choice = promotion_choice;
    api_response.text = strdup(msg->valuestring);
    api_response.response = true;
    api_response.turn = col->valuestring;
    char * opponent_color = (strcmp(play_color, "white") == 0) ? "black" : "white";
   // printf("API moves\n");
    make_move(pieces, terminated_pieces, play_color, origin_x, origin_y, position_x, position_y, zug_counter, halbzug_counter, opponent_color, api_response);
    return api_response;
}

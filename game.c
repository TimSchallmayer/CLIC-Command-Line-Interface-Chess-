#include <stdio.h>
#include "chess.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


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

    // Initialize white pieces
    pieces[0] = (Piece){"Rook", "♖", 0, 7, true, false};
    pieces[1] = (Piece){"Knight", "♘", 1, 7, true, false};
    pieces[2] = (Piece){"Bishop", "♗", 2, 7, true, false};
    pieces[3] = (Piece){"Queen", "♕", 3, 7, true, false};
    pieces[4] = (Piece){"King", "♔", 4, 7, true, false};
    pieces[5] = (Piece){"Bishop", "♗", 5, 7, true, false};
    pieces[6] = (Piece){"Knight", "♘", 6, 7, true, false};
    pieces[7] = (Piece){"Rook", "♖", 7, 7, true, false};
    for (int i = 0; i < 8; i++) {
        pieces[8 + i] = (Piece){"Pawn", "♙", i, 6, true, false};
    }

    // Initialize black pieces
    pieces[16] = (Piece){"Rook", "♜", 0, 0, false, false};
    pieces[17] = (Piece){"Knight", "♞", 1, 0, false, false};
    pieces[18] = (Piece){"Bishop", "♝", 2, 0, false, false};
    pieces[19] = (Piece){"Queen", "♛", 3, 0, false, false};
    pieces[20] = (Piece){"King", "♚", 4, 0, false, false};
    pieces[21] = (Piece){"Bishop", "♝", 5, 0, false, false};
    pieces[22] = (Piece){"Knight", "♞", 6, 0, false, false};
    pieces[23] = (Piece){"Rook", "♜", 7, 0, false, false};
    for (int i = 0; i < 8; i++) {
        pieces[24 + i] = (Piece){"Pawn", "♟", i, 1, false, false};
    }
}
char * introduction() {
    char choice;
    printf("Welcome to Command-Line Chess!\nFor the game instructions, please type 'help'.\n");
    printf("Would you like to play as white, black or random? ");
    char* color = malloc(6 * sizeof(char));
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
            printf("3. Type ctr + c to exit the pogramm.\n");
            printf("4. Make shure to have a utf-8 terminal\n");
            printf("5. Now pick a color: ");
            continue;
        }
        else{
            printf("Invalid choice. Please type 'white' or 'black': ");
            continue;
        }
    }

    
}

bool valid_move(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y, Position passant) {
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
    if (in_check(pieces, color) && strcmp(piece.name, "King") != 0)
    {
        printf("You are in check! You must move your king out of check.\n");
        return false;
    }
    if (!is_move_safe(piece, pieces, origin_x, origin_y, position_x, position_y))
    {
        printf("This move would put your king in check!\n");
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
                else if (position_x == passant.x && position_y == passant.y)
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
                else if (position_x == passant.x && position_y == passant.y)
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
        int diff_x;
        int diff_y;
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
        if (abs(position_x - origin_x) > 1 || abs(position_y - origin_y) > 1) {
            return false;
        }
        if (strcmp(color, "white") == 0)
        {
            for (int i = 0; i < 16; i++)
            {   
                if (strcmp(pieces[i].name, "King") == 0)
                {
                    if (abs(pieces[i].x - position_x) <= 1 && abs(pieces[i].y - position_y) <= 1) {
                        return false;
                    }
                    else {
                        continue;
                    }
                    
                }
                if (strcmp(pieces[i].name, "Pawn") == 0)
                {
                    if(abs(pieces[i].x - position_x) <=1 && pieces[i].y -1 == position_y) {
                        return false;
                    }
                    else {
                        continue;
                    }
                }
                
                
                else if (valid_move(pieces[i], pieces, pieces[i].x, pieces[i].y, position_x, position_y, passant))
                {
                    return false;
                }
                
            }
            return true;
            
        }
        else  {
            for (int i = 16; i < 32; i++)
            {   
                if (strcmp(pieces[i].name, "King") == 0)
                {
                    if (abs(pieces[i].x - position_x) <= 1 && abs(pieces[i].y - position_y) <= 1) {
                        printf("lethal move King");
                        return false;
                    }
                    else {
                        continue;
                    }
                }
                if (strcmp(pieces[i].name, "Pawn") == 0)
                {
                    if(abs(pieces[i].x - position_x) <=1 && pieces[i].y + 1 == position_y) {
                        return false;
                    }
                    else {
                        continue;
                    }
                }
                    
                
                else if (valid_move(pieces[i], pieces, pieces[i].x, pieces[i].y, position_x, position_y, passant))
                {   
                    return false;
                }
                
            }
            return true;
        }
        
        
    }
    else {
        return false;
    }
    
    
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

char * make_fen(Piece* pieces, char * color, int zug_counter, int halbzug_counter, Position passant) {
    char * fen = calloc(200, sizeof(char));
    int passed = 0;    
    for (int i = 0; i < 8; i++)
    {
        passed = 0;
        bool found = false;
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 32; k++)
            {
                if (pieces[k].x == j && pieces[k].y == i)
                {
                    if (passed > 0)
                    {
                        char passed_char = passed + '0';
                        char passed_str[2] = {passed_char, '\0'};
                        strcat(fen, passed_str);
                        passed = 0;
                    }
                    char symbol;
                    if (pieces[k].name == "Knight")
                    {
                        symbol = 'N';    
                    }
                    else {
                        symbol = pieces[k].name[0];
                    }
                    if (pieces[k].is_white)
                    {
                        symbol = tolower(symbol);
                    }
                    char symbol_str[2] = {symbol, '\0'};
                    strcat(fen, symbol_str);
                    found = true;
                    break;
                    
                }
            }
            if (!found){
                    passed +=1;
                    found = false;
                    if (j == 7)
                    {
                        char passed_char = passed + '0';
                        char passed_str[2] = {passed_char, '\0'};
                        strcat(fen, passed_str);                   
                    }
                    
                }
            
        }
        strcat(fen, "/");
        found = false;
        
    }
    if (strcmp(color, "white") == 0)
    {
        strcat(fen, " b");
    }
    else if (strcmp(color, "black") == 0)
    {
        strcat(fen, " w");
    }
    bool whiteK = is_castleling_possible(pieces[20], pieces[23], pieces, "white"); 
    bool whiteQ = is_castleling_possible(pieces[20], pieces[16], pieces, "white");
    bool blackK = is_castleling_possible(pieces[4], pieces[7], pieces, "black"); 
    bool blackQ = is_castleling_possible(pieces[4], pieces[0], pieces, "black");   

    // Jetzt FEN-String aufbauen
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
    if (passant.x >=0 && passant.x <=7 && passant.y >=0 && passant.y <=7)
    {
        char file = passant.x + 'a';
        char rank = passant.y + '1';
        char passant_str[3] = {file, rank, '\0'};
        strcat(fen, " ");
        strcat(fen, passant_str);
    }
    else {
        strcat(fen, "-");
    }
    strcat(fen, " ");
    char halbzug_str[4] = {halbzug_counter + '0', '\0'};
    strcat(fen, halbzug_str);
    strcat(fen, " ");
    char zug_str[4] = {zug_counter + '0', '\0'};
    strcat(fen, zug_str);
    return fen;
}


bool is_sqare_attacked(int x, int y, Piece* pieces, char* color) {
    Position passant;
    for (int i = 0; i < 32; i++) {
        if (!pieces[i].is_white && strcmp(color, "black") == 0 || pieces[i].is_white && strcmp(color, "white") == 0) {
            if (valid_move(pieces[i], pieces, pieces[i].x, pieces[i].y, x, y, passant)) {
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
        
        if (king.y != 0 || rook.y != 0) {
            return false;
        }
    }
    else if (strcmp(color, "black") == 0) {
        if (king.y != 7 || rook.y != 7) {
            return false;
        }
    }

    if (is_sqare_attacked(king.x, king.y, pieces, color)) {
        return false;
    }
    if (king.has_moved || rook.has_moved)
    {
        return false;
    }
    
    int step = 0;
    if (rook.x > king.x) {
        step = 1;
    }
    else if (rook.x < king.x) {
        step = -1;
    }
    for (int x = king.x + step; x != rook.x; x += step) {
        if (is_piece(x, king.y, pieces, "both")) {
            return false;
        }
        if (is_sqare_attacked(x, king.y, pieces, color)) {
            return false;
        }
    }
    return true;
}
void terminate_piece(Piece piece) {
    piece.x = -1;
    piece.y = -1;
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
    return is_sqare_attacked(king.x, king.y, pieces, color);
}
bool is_move_safe(Piece piece, Piece* pieces, int origin_x, int origin_y, int position_x, int position_y) {
    bool is_white = !piece.is_white;
    char * color;
    if (is_white) color = "white";
    else color = "black";

    int original_x = piece.x;
    int original_y = piece.y;
    piece.x = position_x;
    piece.y = position_y;

    bool safe = !in_check(pieces, color);

    piece.x = original_x;
    piece.y = original_y;

    return safe;
}
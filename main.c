#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chess.h"
#include <cjson/cJSON.h>
#include <cjson/cJSON_Utils.h>
#include <curl/curl.h>

int main(int argc, char* argv[]) {
    system("powershell -Command \"[Console]::OutputEncoding = [System.Text.Encoding]::UTF8\"");
    Piece* pieces = malloc(32 * sizeof(Piece));
    Piece* terminated_pieces = malloc(32 * sizeof(Piece));
    Position passant;
    passant.x = -1;
    passant.y = -1;
    int passantr_counter = 0;

    if (pieces == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int zug_counter = 1;
    int halbzug_counter = 0;
    char * play_color = introduction();

    create_pieces(pieces);
    draw_chessboard(pieces, play_color);
    printf("made chessboard\n");
    char* data = make_fen(pieces, play_color, zug_counter, halbzug_counter, passant);
    printf("\nFEN: %s\n", data);
    API_call api_call;
    api_call.fen = data;
    api_call.depth = 12;
    api_call.max_thinking_time = 5000;
    char* json_request = make_json(api_call);
    printf("JSON Request: %s\n", json_request);
    while (1) {
        char piece[7];     
        char position[3];   
        char origin[3];

        printf("Enter piece, origin amd position: ");

        if (scanf("%6s %2s %2s", piece, origin, position) != 3) {
            printf("Invalid input!\n");
            continue;
        }
        printf("\n");
        for (int i = 0; i < 6; i++) {
            piece[i] = tolower(piece[i]);
        }
        if (tolower(position[0]) < 'a' || tolower(position[0]) > 'h' ||
            position[1] < '1' || position[1] > '8') {
            printf("Invalid position!\n");
            printf("Position: %2s \n", position);
            continue;
        }
        if (tolower(origin[0]) < 'a' || tolower(origin[0]) > 'h' ||
            origin[1] < '1' || origin[1] > '8') {
            printf("Invalid origin!\n");
            printf("Origin: %2s \n", origin);
            continue;
        }

        int position_x = tolower(position[0]) - 'a';
        int position_y = position[1] - '1';
        int origin_x = tolower(origin[0]) - 'a'; 
        int origin_y = tolower(origin[1]) - '1';
        Piece choosen_piece;
        
        for (int i = 0; i < 32; i++)
        {
            if (pieces[i].x == origin_x && pieces[i].y == origin_y && (pieces[i].is_white == false && strcmp(play_color, "white") == 0 || pieces[i].is_white == true && strcmp(play_color, "black") == 0))
            {
                choosen_piece = pieces[i];
            }
            continue;
        }
        
        if (origin_x == position_x && origin_y == position_y ){
            printf("Invalid position! \n");
            continue;
        }
        if (!valid_move(choosen_piece, pieces, origin_x, origin_y, position_x, position_y, passant))
        {
            printf("Invalid move!\n");
            continue;
        }
        if (is_piece(position_x, position_y, pieces, "both") || position_x == passant.x && position_y == passant.y)
        {
            if (position_x == passant.x && passant.y == position_y && strcmp(choosen_piece.name, "pawn") == 0)
            {
                int index = which_piece(pieces, position_x, passant.y + (choosen_piece.is_white ? -1 : 1));
                passant.y = -1;
                passant.x = -1;
            }
            else {
                int index = which_piece(pieces, position_x, position_y);
                if (index != -1) {
                    printf("%s captures %s\n", choosen_piece.name, pieces[index].name);
                    terminate_piece(pieces[index]);
                }
            }

        }
        if (in_check(pieces, play_color) && strcmp(choosen_piece.name, "King") != 0)
        {
            printf("You are in check! You must move your king out of check.\n");
            continue;
        }
        if (!is_move_safe(choosen_piece, pieces, origin_x, origin_y, position_x, position_y))
        {
            printf("This move would put your king in check!\n");
            continue;
        }
        
        int i = which_piece(pieces, origin_x, origin_y);
        if (i == -1) {
            printf("Unknown piece, origin or position.\n");
            continue;
        }
        printf("Moving %s to %s\n", pieces[i].name, position);
        if (strcmp(pieces[i].name, "Pawn") == 0)
        {
            halbzug_counter = 0;
            if (position_y == (pieces[i].is_white ? 1 : 8))
            {   
                bool invalid = true;
                while (invalid)
                {
                    printf("Pawn promoted to:");
                    char promotion_choice[7];
                    scanf("%6s", promotion_choice);
                    promotion_choice[0] = toupper(promotion_choice[0]);
                    pieces[i].name = promotion_choice;
                    if (strcmp(promotion_choice, "Knight") == 0)
                    {
                        pieces[i].symbol = pieces[i].is_white ? "♘" : "♞";
                        invalid = false;
                    }
                    else if (strcmp(promotion_choice, "Bishop") == 0)
                    {
                        pieces[i].symbol = pieces[i].is_white ? "♗" : "♝";
                        invalid = false;
                    }
                    else if (strcmp(promotion_choice, "Rook") == 0)
                    {
                        pieces[i].symbol = pieces[i].is_white ? "♖" : "♜";
                        invalid = false;
                    }
                    else if (strcmp(promotion_choice, "Queen") == 0)
                    {
                        pieces[i].symbol = pieces[i].is_white ? "♕" : "♛";
                        invalid = false;
                    }
                    else {
                        printf("Invalid promotion choice!\n");
                        invalid = true;
                }      
            }
            
            if (abs(position_y - origin_y) == 2) {
                passant.x = origin_x;
                passant.y = (origin_y + position_y) / 2;  
            } else {
                passant.x = -1;
                passant.y = -1;
            }
        }
        pieces[i].x = position_x;
        pieces[i].y = position_y;
        pieces[i].has_moved = true;

        draw_chessboard(pieces, play_color);
        halbzug_counter++;
        // Hier kommt noch der API coll
        zug_counter++;
        if (!(abs(position_y - origin_y) == 2 && strcmp(piece, "pawn") == 0)) {
            passant.x = -1;
            passant.y = -1;
        }
    }
    free(pieces);
    free(terminated_pieces);
    return 0;
}}
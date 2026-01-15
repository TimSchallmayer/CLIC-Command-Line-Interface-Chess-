#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chess.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>

int main() {
    system("powershell -Command \"[Console]::OutputEncoding = [System.Text.Encoding]::UTF8\"");
    Piece* pieces = malloc(32 * sizeof(Piece));
    if (pieces == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int zug_counter = 1;
    int halbzug_counter = 0;
    int difficulty = 12;
    char * play_color = introduction(&difficulty);
    char * opponent_color = (strcmp(play_color, "white") == 0) ? "black" : "white";
    create_pieces(pieces);
    draw_chessboard(pieces, play_color);
    bool game_over = false;
    while (!game_over) {
        
        if (strcmp(play_color, "white") == 0)
        {
            char piece[7];     
            char position[3];   
            char origin[3];

            printf("Enter piece, origin and position: ");

            if (scanf("%6s %2s %2s", piece, origin, position) != 3) {
                printf("Invalid input!\n");
                continue;
            }
            if (strcmp(piece, "quit") == 0) {
                printf("Exiting game...\n");
                game_over = true;
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
        //    printf("valid INput\n");
            int position_x = tolower(position[0]) - 'a';
            int position_y = position[1] - '1';
            int origin_x = tolower(origin[0]) - 'a'; 
            int origin_y = tolower(origin[1]) - '1';
            Piece choosen_piece;
            bool found = false;
            for (int i = 0; i < 32; i++) {
                if (pieces[i].x == origin_x && pieces[i].y == origin_y && ((pieces[i].is_white == false && (strcmp(play_color, "white") == 0 ||
                (pieces[i].is_white == true && (strcmp(play_color, "black") == 0 )))))) 
                {
                    choosen_piece = pieces[i];
                    found = true;
                    break;
                }
            }
     
            if (!found) {
                printf("No matching piece found at origin.\n");
                continue;
            }
        
            if (origin_x == position_x && origin_y == position_y) {
                printf("Invalid position!\n");
                continue;
            }

            if (!valid_move(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                printf("Invalid move!\n");
                continue;
            }

            if (!is_move_safe(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                printf("Either your are in check or this move would put in it!\n");
                continue;
            }
           
            int i = which_piece(pieces, origin_x, origin_y);
            if (i == -1) {
                printf("Unknown piece.\n");
                continue;
            }
           
            if (is_piece(position_x, position_y, pieces, "both")) 
            {
                int index = -1;
                index = which_piece(pieces, position_x, position_y);
                if (index != -1) {
                    printf("           %s captures %s\n", choosen_piece.name, pieces[index].name);
                    terminate_piece(&pieces[index]);
                }
            }
           // printf("Making move...\n");
            API_response user_respsonse;
            user_respsonse.response = false;
            user_respsonse.mate = 100;
            user_respsonse.turn = strcmp(play_color, "white") == 0 ? "b" : "w";
            if (abs(origin_x - position_x) == 2 && strcmp(choosen_piece.name, "King") == 0)
            {
                printf("Castleling detected.\n");
                if (origin_x > position_x)
                {
                    int index_rook = which_piece(pieces, 0, origin_y);
                    if (index_rook != -1) {
                        pieces[index_rook].x = position_x +1;
                    }
                    printf("Rook moved during castleling1.\n");
                }
                else {
                    int index_rook = which_piece(pieces, 7, origin_y);
                    if (index_rook != -1) {
                        pieces[index_rook].x = position_x -1;
                    }
                    printf("Rook moved during castleling.\n");
                
            }
        }
            make_move(pieces, play_color, origin_x, origin_y, position_x, position_y, &zug_counter, &halbzug_counter, NULL, user_respsonse);
            pieces[i].has_moved = true;
            //printf("making fen");
            char* data = make_fen(pieces, play_color, zug_counter, halbzug_counter);
           // printf("fen made: %s", data);
            API_call api_call;
            api_call.fen = data;
            api_call.depth = difficulty;
            api_call.max_thinking_time = 5000;
            char* json_request = make_json(api_call);
            char * json_answer = curl(json_request);
           // printf("answer received: %s \n", json_answer);
            cJSON* json_parsed = cJSON_Parse(json_answer);
            api_move(json_parsed, pieces, play_color, &zug_counter, &halbzug_counter);
            zug_counter++;
            free(data);
            free(json_request);
            free(json_answer);
            cJSON_Delete(json_parsed);
        }
        else {
          //  printf("Waiting for opponent's move...\n");
            char* data = make_fen(pieces, play_color, zug_counter, halbzug_counter);
            API_call api_call;
            api_call.fen = data;
            api_call.depth = difficulty;
            api_call.max_thinking_time = 5000;
            char* json_request = make_json(api_call);
            char * json_answer = curl(json_request);
            cJSON* json_parsed = cJSON_Parse(json_answer);
            API_response api_response = api_move(json_parsed, pieces, play_color, &zug_counter, &halbzug_counter);
            free(data);
            free(json_request);
            free(json_answer);
            cJSON_Delete(json_parsed);


         //   printf("Opponent moved.\n");
            char piece[7];     
            char position[3];   
            char origin[3];

            bool valid_input = false;
            while (!valid_input)
            {
                printf("Enter piece, origin and position: ");

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
             //   printf("Making move...\n");
                Piece choosen_piece;
                bool found = false;
                for (int i = 0; i < 32; i++) {
                    if (pieces[i].x == origin_x && pieces[i].y == origin_y && ((pieces[i].is_white == false && (strcmp(play_color, "white") == 0 ||
                (pieces[i].is_white == true && (strcmp(play_color, "black") == 0 )))))) 
                    {
                        choosen_piece = pieces[i];
                        found = true;
                        break;
                    }
                }

            
                if (!found) {
                    printf("No matching piece found at origin.\n");
                    continue;
                }
          
                if (origin_x == position_x && origin_y == position_y) {
                    printf("Invalid position!\n");
                    continue;
                }
             
                if (!valid_move(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                    printf("Invalid move!\n");
                    continue;
                }
        
                if (!is_move_safe(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                    printf("Either your are in check or this move would put in it!\n");
                    continue;
                }
               
                int i = which_piece(pieces, origin_x, origin_y);
                if (i == -1) {
                    printf("Unknown piece.\n");
                    continue;
                }
                /* Funktioniert nicht, da die benutze API en passant nicht richtig behandelt
                if (abs(position_y - origin_y) == 2 && strcmp(choosen_piece.name, "Pawn") == 0) { 
                    printf("En Passant possible!\n");

                    int ep_x = origin_x;
                    int ep_y = (origin_y + position_y) / 2;
                    bool can_capture = false;
                    printf("%i %i\n", ep_x, ep_y);

                    for (int i = 0; i < 32; i++) {
                        if (pieces[i].x != -1 && pieces[i].y != -1 && strcmp(pieces[i].name, "Pawn") == 0) {
                            printf("Checking piece at %d,%d\n", pieces[i].x, pieces[i].y);
                            if (pieces[i].is_white != choosen_piece.is_white && pieces[i].y == ep_y +1 || pieces[i].y == ep_y -1) {
                                if (pieces[i].x == ep_x - 1 || pieces[i].x == ep_x + 1) {
                                    can_capture = true;
                                    printf("En Passant possible!\n");
                                }
                            }
                        }
                    }

                    if (can_capture) {
                            passant.x = ep_x;
                            passant.y = ep_y;
                    } else {
                            passant.x = -1;
                            passant.y = -1;
                    }
                }
                else {
                    printf("En Passant not possible!\n");
                    passant.x = -1;
                    passant.y = -1;
                    }*/
                if (is_piece(position_x, position_y, pieces, "both")) 
                {
                    int index = -1;
                    index = which_piece(pieces, position_x, position_y);
                    if (index != -1) {
                        printf("           %s captures %s\n", choosen_piece.name, pieces[index].name);
                        terminate_piece(&pieces[index]);
                    }
                }
                API_response user_respsonse;
                user_respsonse.response = false;
                user_respsonse.mate = 100;
                user_respsonse.turn = strcmp(play_color, "white") == 0 ? "b" : "w";
                if (abs(origin_x - position_x == 2) && strcmp(choosen_piece.name, "King") == 0)
                {
                    if (origin_x > position_x)
                    {
                        int index_rook = which_piece(pieces, 0, origin_y);
                        if (index_rook != -1) {
                            pieces[index_rook].x = position_x +1;
                        }
                        printf("Rook moved during castleling1.\n");
                    }
                    else {
                        int index_rook = which_piece(pieces, 7, origin_y);
                        if (index_rook != -1) {
                            pieces[index_rook].x = position_x -1;
                        }
                        printf("Rook moved during castleling.\n");
                    
                }
                }
                make_move(pieces, play_color, origin_x, origin_y, position_x, position_y, &zug_counter, &halbzug_counter, NULL, user_respsonse);
             //   printf("Move made.\n");
                pieces[i].has_moved = true;
                valid_input = true;
            }
            zug_counter++;

        }
    
    
    }
    for (int i = 8; i < 16; i++) {
        free(pieces[i].name);
        free(pieces[i].symbol);
    }
    for (int i = 24; i < 32; i++) {
        free(pieces[i].name);
        free(pieces[i].symbol);
    }
    free(pieces);
    return 0;
}

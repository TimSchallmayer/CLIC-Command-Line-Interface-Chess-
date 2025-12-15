#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chess.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>

int main(int argc, char* argv[]) {
    system("powershell -Command \"[Console]::OutputEncoding = [System.Text.Encoding]::UTF8\"");
    Piece* pieces = malloc(32 * sizeof(Piece));
    Piece* terminated_pieces = malloc(32 * sizeof(Piece));

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
    int c;
    while (!game_over) {
        
        if (strcmp(play_color, "white") == 0)
        {
            char piece[7];     
            char position[3];   
            char origin[3];

            printf("Enter piece, origin amd position: ");

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


            if (in_check(pieces, play_color) && !is_move_safe(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                printf("You are in check! Move your king out of check.\n");
                continue;
            }

            if (!is_move_safe(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                printf("This move would put your king in check!\n");
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

            API_response user_respsonse;
            user_respsonse.response = false;
            user_respsonse.mate = 100;
            user_respsonse.turn = strcmp(play_color, "white") == 0 ? "b" : "w";
            make_move(pieces, terminated_pieces, play_color, origin_x, origin_y, position_x, position_y, &zug_counter, &halbzug_counter, NULL, user_respsonse);
       //     printf("making fen");
            char* data = make_fen(pieces, play_color, zug_counter, halbzug_counter);
        //    printf("fen made");
            API_call api_call;
            api_call.fen = data;
            api_call.depth = difficulty;
            api_call.max_thinking_time = 5000;
            char* json_request = make_json(api_call);
            char * json_answer = curl(json_request);
       //     printf("answer received");
            api_move(cJSON_Parse(json_answer), pieces, play_color, &zug_counter, &halbzug_counter, terminated_pieces);
            zug_counter++;
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
            API_response api_response = api_move(cJSON_Parse(json_answer), pieces, play_color, &zug_counter, &halbzug_counter, terminated_pieces);
         //   printf("Opponent moved.\n");
            char piece[7];     
            char position[3];   
            char origin[3];

            bool valid_input = false;
            while (!valid_input)
            {
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
             //   printf("Making move...\n");
                Piece choosen_piece;
                bool found = false;
                for (int i = 0; i < 32; i++) {
                    if (pieces[i].x == origin_x && pieces[i].y == origin_y && ((pieces[i].is_white == false && (strcmp(play_color, "white") == 0)) ||
                    (pieces[i].is_white == true && (strcmp(play_color, "black") == 0)))) 
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
               // printf("Chosen piece: %s \n", choosen_piece.name);
                if (origin_x == position_x && origin_y == position_y) {
                    printf("Invalid position!\n");
                    continue;
                }

                if (!valid_move(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                    printf("Invalid move!\n");
                    continue;
                }

                if (!is_move_safe(choosen_piece, pieces, origin_x, origin_y, position_x, position_y)) {
                    printf("This move would put your king in check!\n");
                    continue;
                }

                int i = which_piece(pieces, origin_x, origin_y);
                if (i == -1) {
                    printf("Unknown piece.\n");
                    continue;
                }/* Funktioniert nicht, da die benutze API en passant nicht richtig behandelt
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
                make_move(pieces, terminated_pieces, play_color, origin_x, origin_y, position_x, position_y, &zug_counter, &halbzug_counter, NULL, user_respsonse);
             //   printf("Move made.\n");
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
    free(terminated_pieces);
    return 0;
}

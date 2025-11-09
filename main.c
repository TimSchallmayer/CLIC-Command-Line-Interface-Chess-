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
            make_move(pieces, terminated_pieces, play_color, origin_x, origin_y, position_x, position_y, &zug_counter, &halbzug_counter, &passant, NULL);
            printf("making fen");
            char* data = make_fen(pieces, play_color, zug_counter, halbzug_counter, passant);
            printf("fen made");
            API_call api_call;
            api_call.fen = data;
            api_call.depth = 12;
            api_call.max_thinking_time = 5000;
            char* json_request = make_json(api_call);
            char * json_answer = curl(json_request);
            printf("answer received");
            api_move(cJSON_Parse(json_answer), pieces, play_color, &zug_counter, &halbzug_counter, &passant, terminated_pieces);
        }
        else {
            printf("Waiting for opponent's move...\n");
            char* data = make_fen(pieces, play_color, zug_counter, halbzug_counter, passant);
            API_call api_call;
            api_call.fen = data;
            api_call.depth = 12;
            api_call.max_thinking_time = 5000;
            char* json_request = make_json(api_call);
            char * json_answer = curl(json_request);
            api_move(cJSON_Parse(json_answer), pieces, play_color, &zug_counter, &halbzug_counter, &passant, terminated_pieces);
            printf("Opponent moved.\n");
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
                printf("Making move...\n");
                make_move(pieces, terminated_pieces, play_color, origin_x, origin_y, position_x, position_y, &zug_counter, &halbzug_counter, &passant, NULL);
                printf("Move made.\n");
                valid_input = true;
            }
            

        }
    
    
    }
    free(pieces);
    free(terminated_pieces);
    return 0;
}
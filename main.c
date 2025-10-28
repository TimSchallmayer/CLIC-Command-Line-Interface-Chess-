#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chess.h"

int main(int argc, char* argv[]) {

    Piece* pieces = malloc(32 * sizeof(Piece));
    if (pieces == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    bool play_color_white = introduction();;
    create_pieces(pieces);
    draw_chessboard(pieces);

    while (1) {
        char piece[6];     
        char position[3];   

        printf("Enter piece and position: ");
        if (scanf("%5s %2s", piece, position) != 2) {
            printf("Invalid input!\n");
            break; // oder continue
        }

        for (int i = 0; i < 6; i++) {
            piece[i] = tolower(piece[i]);
        }

        if (strcmp(position, "exit") == 0 || strcmp(piece, "exit") == 0) {
            break;
        }
        if (tolower(position[0]) < 'a' || tolower(position[0]) > 'h' ||
            position[1] < '1' || position[1] > '8') {
            printf("Invalid position!\n");
            printf("Position: %2s\n", position);
            continue;
        }

        int posiotion_x = tolower(position[0]) - 'a';
        int position_y = position[1] - '1';

        if (piece[0] == 'r' || strcmp(piece, "rook") == 0) {
            printf("Moving Rook to %s\n", position);
            if (!play_color_white) {
                pieces[0].x = posiotion_x;
                pieces[0].y = position_y;
            } else {
                pieces[16].x = posiotion_x;
                pieces[16].y = position_y;
            }
        }
        else if (tolower(piece[0]) == 'p' || strcmp(piece, "pawn") == 0) {
            printf("Moving Pawn to %s\n", position);
            if (!play_color_white) {
                pieces[8].x = posiotion_x;
                pieces[8].y = position_y;
            } else {
                pieces[24].x = posiotion_x;
                pieces[24].y = position_y;
            }
        }
        else if (piece[0] == 'n' || strcmp(piece, "knight") == 0) {
            printf("Moving Knight to %s\n", position);
            if (!play_color_white) {
                pieces[1].x = posiotion_x;
                pieces[1].y = position_y;
            } else {
                pieces[17].x = posiotion_x;
                pieces[17].y = position_y;
            }
        }
        else if (piece[0] == 'b' || strcmp(piece, "bishop") == 0) {
            printf("Moving Bishop to %s\n", position);
            if (!play_color_white) {
                pieces[2].x = posiotion_x;
                pieces[2].y = position_y;
            } else {
                pieces[18].x = posiotion_x;
                pieces[18].y = position_y;
            }
        }
        else if (piece[0] == 'q' || strcmp(piece, "queen") == 0) {
            printf("Moving Queen to %s\n", position);
            if (!play_color_white) {
                pieces[3].x = posiotion_x;
                pieces[3].y = position_y;
            } else {
                pieces[19].x = posiotion_x;
                pieces[19].y = position_y;
            }
        }
        else if (piece[0] == 'k' || strcmp(piece, "king") == 0) {
            printf("Moving King to %s\n", position);
            if (!play_color_white) {
                pieces[4].x = posiotion_x;
                pieces[4].y = position_y;
            } else {
                pieces[20].x = posiotion_x;
                pieces[20].y = position_y;
            }
        }
        else {
            printf("Unkown peice or posiition.");
            continue;
        }
        draw_chessboard(pieces);
    }
    free(pieces);
    return 0;
}


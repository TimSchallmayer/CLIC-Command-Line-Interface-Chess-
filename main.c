#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chess.h"

int main(int argc, char* argv[]) {
    system("powershell -Command \"[Console]::OutputEncoding = [System.Text.Encoding]::UTF8\"");
    Piece* pieces = malloc(32 * sizeof(Piece));
    if (pieces == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    int zug_counter = 0;
    int halbzug_counter = 0;
    char * play_color = introduction();
    create_pieces(pieces);
    draw_chessboard(pieces, play_color);
    char* data = make_fen(pieces, play_color, zug_counter, halbzug_counter);
    printf("%s", data);
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
        if (!valid_move(choosen_piece, pieces, origin_x, origin_y, position_x, position_y))
        {
            printf("Invalid move!\n");
            continue;
        }
        

        if (piece[0] == 'r' || strcmp(piece, "rook") == 0) {
            printf("   Moving Rook to %s\n", position);
            if (strcmp(play_color, "black") == 0) {
                if (pieces[0].x == origin_x && pieces[0].y == origin_y) {
                    pieces[0].x = position_x;
                    pieces[0].y = position_y;
                    pieces[0].has_moved = true;
                }
                else if (pieces[7].x == origin_x && pieces[7].y == origin_y) {
                    pieces[7].x = position_x;
                    pieces[7].y = position_y;
                    pieces[7].has_moved = true;
                }
            } else {
                if (pieces[16].x == origin_x && pieces[16].y == origin_y) {
                    pieces[16].x = position_x;
                    pieces[16].y = position_y;
                    pieces[16].has_moved = true;
                }
                else if (pieces[23].x == origin_x && pieces[23].y == origin_y) {
                    pieces[23].x = position_x;
                    pieces[23].y = position_y;
                    pieces[23].has_moved = true;
                }
            }
        }
        else if (tolower(piece[0]) == 'p' || strcmp(piece, "pawn") == 0) {
            halbzug_counter = 0;
            printf("   Moving Pawn to %s\n", position);
            if (strcmp(play_color, "black") == 0) {
                for (int i = 8; i < 16; i++) {
                    if (pieces[i].x == origin_x && pieces[i].y == origin_y) {
                        pieces[i].x = position_x;
                        pieces[i].y = position_y;
                        pieces[i].has_moved = true;
                        break;
                    }
                }
            } else {
                for (int i = 24; i < 32; i++) {
                    if (pieces[i].x == origin_x && pieces[i].y == origin_y) {
                        pieces[i].x = position_x;
                        pieces[i].y = position_y;
                        pieces[i].has_moved = true;
                        break;
                    }
                }
            }
        }

        else if (piece[0] == 'n' || strcmp(piece, "knight") == 0) {
            printf("   Moving Knight to %s\n", position);
            if (strcmp(play_color, "black") == 0) {
                if (pieces[1].x == origin_x && pieces[1].y == origin_y) {
                    pieces[1].x = position_x;
                    pieces[1].y = position_y;
                    pieces[1].has_moved = true;
                } else if (pieces[6].x == origin_x && pieces[6].y == origin_y) {
                    pieces[6].x = position_x;
                    pieces[6].y = position_y;
                    pieces[6].has_moved = true;
                }
            } else {
                if (pieces[17].x == origin_x && pieces[17].y == origin_y) {
                    pieces[17].x = position_x;
                    pieces[17].y = position_y;
                    pieces[17].has_moved = true;
                } else if (pieces[22].x == origin_x && pieces[22].y == origin_y) {
                    pieces[22].x = position_x;
                    pieces[22].y = position_y;
                    pieces[22].has_moved = true;
                }
            }
        }

        else if (piece[0] == 'b' || strcmp(piece, "bishop") == 0) {
            printf("   Moving Bishop to %s\n", position);
            if (strcmp(play_color, "black") == 0) {
                if (pieces[2].x == origin_x && pieces[2].y == origin_y) {
                    pieces[2].x = position_x;
                    pieces[2].y = position_y;
                    pieces[2].has_moved = true;
                } else if (pieces[5].x == origin_x && pieces[5].y == origin_y) {
                    pieces[5].x = position_x;
                    pieces[5].y = position_y;
                    pieces[5].has_moved = true;
                }
            } else {
                if (pieces[18].x == origin_x && pieces[18].y == origin_y) {
                    pieces[18].x = position_x;
                    pieces[18].y = position_y;
                    pieces[18].has_moved = true;
                } else if (pieces[21].x == origin_x && pieces[21].y == origin_y) {
                    pieces[21].x = position_x;
                    pieces[21].y = position_y;
                    pieces[21].has_moved = true;
                }
            }
        }

        else if (piece[0] == 'q' || strcmp(piece, "queen") == 0) {
            printf("   Moving Queen to %s\n", position);
            if (strcmp(play_color, "black") == 0) {
                if (pieces[3].x == origin_x && pieces[3].y == origin_y) {
                    pieces[3].x = position_x;
                    pieces[3].y = position_y;
                    pieces[3].has_moved = true;
                }
            } else {
                if (pieces[19].x == origin_x && pieces[19].y == origin_y) {
                    pieces[19].x = position_x;
                    pieces[19].y = position_y;
                    pieces[19].has_moved = true;
                }
            }
        }

        else if (piece[0] == 'k' || strcmp(piece, "king") == 0) {
            printf("   Moving King to %s\n", position);
            if (strcmp(play_color, "black") == 0) {
                if (pieces[4].x == origin_x && pieces[4].y == origin_y) {
                    pieces[4].x = position_x;
                    pieces[4].y = position_y;
                    pieces[4].has_moved = true;
                }
            } else {
                if (pieces[20].x == origin_x && pieces[20].y == origin_y) {
                    pieces[20].x = position_x;
                    pieces[20].y = position_y;
                    pieces[20].has_moved = true;
                }
            }
        }

        else {
            printf("Unknown piece, origin or position.\n");
            continue;
        }

        draw_chessboard(pieces, play_color);
        halbzug_counter++;
        // Hier kommt noch der API coll
        zug_counter++;
    free(pieces);
    return 0;
}
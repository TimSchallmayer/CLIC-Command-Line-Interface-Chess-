#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//structs
typedef struct {
    char* name;
    char* symbol;
    int x;
    int y;
    bool is_white;
} Piece;
//function prototypes
void draw_chessboard(Piece* pieces);
void create_pieces(Piece* pieces);

int main(int argc, char* argv[]) {

    Piece* pieces = malloc(32 * sizeof(Piece));
    if (pieces == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to the Chess Game!\n Which color do you want to play? (white/black): ");
    char color[6];
    bool play_color_white;
    scanf("%5s", color);
    if (strcmp(color, "white") == 0 || strcmp(color, "w") == 0 || strcmp(color, "White") == 0 || strcmp(color, "W") == 0) {
        play_color_white = true;
    } else if (strcmp(color, "black") == 0 || strcmp(color, "b") == 0 || strcmp(color, "Black") == 0 || strcmp(color, "B") == 0) {
        play_color_white = false;
    } else {
        printf("Invalid color choice!\n");
        free(pieces);
        return 1;
    }   

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
        if (strcmp(position, "exit") == 0 || strcmp(piece, "exit") == 0) {
            break;
        }
        if (strcmp(piece, "Rook") != 0 && strcmp(piece, "Pawn") != 0 &&
            strcmp(piece, "Knight") != 0 && strcmp(piece, "Bishop") != 0 &&
            strcmp(piece, "Queen") != 0 && strcmp(piece, "King") != 0) {
            printf("Unknown piece!\n");
            continue;
        }
        if (position[0] < 'a' || position[0] > 'h' ||
            position[1] < '1' || position[1] > '8') {
            printf("Invalid position!\n");
            continue;
        }
        int posiotion_x = position[0] - 'a';
        int position_y = position[1] - '1';

        if (piece[0] == 'R' || strcmp(piece, "Rook") == 0) {
            printf("Moving Rook to %s\n", position);
            if (play_color_white) {
                pieces[0].x = posiotion_x;
                pieces[0].y = position_y;
            } else {
                pieces[16].x = posiotion_x;
                pieces[16].y = position_y;
            }
        }
        draw_chessboard(pieces);
    }
    free(pieces);
    return 0;
}

void draw_chessboard(Piece* pieces) {
    printf("  +----+----+----+----+----+----+----+----+\n");
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
    printf("    Ａ   Ｂ   Ｃ   Ｄ   Ｅ   Ｆ   Ｇ   Ｈ\n");
}
void create_pieces(Piece* pieces) {

    // Initialize white pieces
    pieces[0] = (Piece){"Rook", "♖", 0, 0, true};
    pieces[1] = (Piece){"Knight", "♘", 1, 0, true};
    pieces[2] = (Piece){"Bishop", "♗", 2, 0, true};
    pieces[3] = (Piece){"Queen", "♕", 3, 0, true};
    pieces[4] = (Piece){"King", "♔", 4, 0, true};
    pieces[5] = (Piece){"Bishop", "♗", 5, 0, true};
    pieces[6] = (Piece){"Knight", "♘", 6, 0, true};
    pieces[7] = (Piece){"Rook", "♖", 7, 0, true};
    for (int i = 0; i < 8; i++) {
        pieces[8 + i] = (Piece){"Pawn", "♙", i, 1, true};
    }

    // Initialize black pieces
    pieces[16] = (Piece){"Rook", "♜", 0, 7, false};
    pieces[17] = (Piece){"Knight", "♞", 1, 7, false};
    pieces[18] = (Piece){"Bishop", "♝", 2, 7, false};
    pieces[19] = (Piece){"Queen", "♛", 3, 7, false};
    pieces[20] = (Piece){"King", "♚", 4, 7, false};
    pieces[21] = (Piece){"Bishop", "♝", 5, 7, false};
    pieces[22] = (Piece){"Knight", "♞", 6, 7, false};
    pieces[23] = (Piece){"Rook", "♜", 7, 7, false};
    for (int i = 0; i < 8; i++) {
        pieces[24 + i] = (Piece){"Pawn", "♟", i, 6, false};
    }
}
#include <stdio.h>
#include "chess.h"
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


void draw_chessboard(Piece* pieces, bool iswhite) {
    printf("  +----+----+----+----+----+----+----+----+\n");
    if (!iswhite) {
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
    pieces[0] = (Piece){"Rook", "♖", 0, 7, true};
    pieces[1] = (Piece){"Knight", "♘", 1, 7, true};
    pieces[2] = (Piece){"Bishop", "♗", 2, 7, true};
    pieces[3] = (Piece){"Queen", "♕", 3, 7, true};
    pieces[4] = (Piece){"King", "♔", 4, 7, true};
    pieces[5] = (Piece){"Bishop", "♗", 5, 7, true};
    pieces[6] = (Piece){"Knight", "♘", 6, 7, true};
    pieces[7] = (Piece){"Rook", "♖", 7, 7, true};
    for (int i = 0; i < 8; i++) {
        pieces[8 + i] = (Piece){"Pawn", "♙", i, 6, true};
    }

    // Initialize black pieces
    pieces[16] = (Piece){"Rook", "♜", 0, 0, false};
    pieces[17] = (Piece){"Knight", "♞", 1, 0, false};
    pieces[18] = (Piece){"Bishop", "♝", 2, 0, false};
    pieces[19] = (Piece){"Queen", "♛", 3, 0, false};
    pieces[20] = (Piece){"King", "♚", 4, 0, false};
    pieces[21] = (Piece){"Bishop", "♝", 5, 0, false};
    pieces[22] = (Piece){"Knight", "♞", 6, 0, false};
    pieces[23] = (Piece){"Rook", "♜", 7, 0, false};
    for (int i = 0; i < 8; i++) {
        pieces[24 + i] = (Piece){"Pawn", "♟", i, 1, false};
    }
}
bool introduction() {
    char choice;
    printf("Welcome to Command-Line Chess!\nFor the game instructions, please type 'help'.\n");
    printf("Would you like to play as white, black or random? ");
    char color[6];
    while (true) {
        scanf("%5s", color);
        for (int i = 0; color[i]; i++) {
            color[i] = tolower(color[i]);
        }
        printf("\n");
        if (strcmp(color, "white") == 0 || color[0] == 'w') {
            printf("                 Playing white.\n");
            return true;
        } else if (strcmp(color, "black") == 0 || color[0] == 'b') {
            printf("                 Playing black.\n");
            return false;
        } else if (strcmp(color, "random") == 0 || color[0] =='r')
        {
            srand(time(NULL)); 
            bool is_white = rand() % 2;
            if (is_white) printf("                 Playing white.\n");
            else printf("                 Playing black.\n"); 
            return is_white;
        }
         
        
        else if (strcmp(color,"help") == 0 ||  color[0] == 'h') {
            printf("GAME INTRODUCTIONS: \n");
            printf("1. You play as the color you choose (white, black or random) or respectively (w, b, r).\n");
            printf("2. You can move a piece by stating the name, origin and position to where it schould go. For Example: Rook A8 A5.\nYou can also use the fist Letter of each piece (R A8 A5). The coordinates and pieces are not case sensitive.\n");
            printf("3. Type exit to exit the pogramm.\n");
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

bool valid_move(Piece piece, Piece* pieces) {
    ;
}
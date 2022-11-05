#include "cards.h"
#include <stdbool.h>
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

typedef struct game_board {
  card table[7][SUIT_SIZE];      // cards not on the table
  card foundation[4][SUIT_SIZE]; // piles for foundation 2 win
  int foundation_len[4];         // What index each fonudation is @
  card **deck;                   // All 52 cards in the deck
  bool used[52];                 // True if card has been used
} game_board;

game_board *init_game_board();

void test();

#endif

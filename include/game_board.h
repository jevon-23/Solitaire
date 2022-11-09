#include "cards.h"
#include <stdbool.h>
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#define MAX_PILE_LEN SUIT_SIZE

enum pile_type {
    DECK_PILE, TABLE_PILE, FOUNDATION_PILE
};

typedef struct pile {
  card *cards;        // Pile of the cards that we have
  int len;             // The current length of the pile
  int max_len;         // The max length this pile can reach
  enum pile_type type; // The type of pile
} pile;

typedef struct game_board {
  pile table[7];      // cards not on the table
  pile foundation[4]; // piles for foundation 2 win
  int foundation_len[4];         // What index each fonudation is @
  pile *deck;                   // All 52 cards in the deck
  bool used[52];                 // True if card has been used
} game_board;


game_board *init_game_board();

void test();
#endif

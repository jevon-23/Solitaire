#include "cards.h"
#include <stdbool.h>
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#define MAX_PILE_LEN SUIT_SIZE
#define MAX_STOCK_LEN 24 // (52 - (7 + 6 + 5 + 4 + 3 + 2 + 1))

enum pile_type { DECK_PILE, TABLE_PILE, FOUNDATION_PILE, STOCK_PILE , STOCK_PILE_REC};

typedef struct pile {
  card *cards;         // Pile of the cards that we have
  int len;             // The current length of the pile
  int max_len;         // The max length this pile can reach
  int index;           // What number card we are at (used 4 stock)
  enum pile_type type; // The type of pile
} pile;

typedef struct game_board {
  pile table[8];            // cards not on the table
  pile foundation[4];       // piles for foundation 2 win
  pile *stock;              // Cards that have not been used in foundation
  pile *stock_recycle;      // Recycle bin for the stock
  card *top_stock_card;     // Card the player has access to on the stock
  card *top_three_stock[3]; // Next 3 cards on the stock
  pile *deck;               // All 52 cards in the deck
  bool used[52];            // True if card has been used
} game_board;

game_board *init_game_board();
card *pile_get_card(pile *p, int index);
void print_game_board(game_board *gb);
card *draw_one(game_board *gb);
bool transfer_pile(pile *dst, pile *src, int num_transfer);
void test();
card *pile_pop_upto(pile *p, int n);
#endif

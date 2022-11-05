#include "../../include/cards.h"
#include "../../include/game_board.h"
#include "../../include/utils.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Gameboard.c. How we are going to set up
 * the gameboard. Not sure what all should
 * go in here yet, but will build out as time
 * goes
 */

void print_table(game_board *gb) {
  printf("printing out the current table of the game\n");
  for (int pile_no = 0; pile_no < 8; pile_no++) {
    printf("pile_no: %d\n", pile_no);
    for (int i = 0; i < pile_no; i++) {
      print_card(&gb->table[pile_no][i]);
    }
  }
}

int rand_no() { return rand() % DECK_SIZE; }

/* Pile_no between 1-7
 */
card *make_pile(game_board *gb, int pile_no) {

  /* Valid pile number? (1-indexed) */
  if (pile_no < 1 || pile_no > 7) {
    error("Invalid pile length\n", -1);
    return NULL;
  }

  /* Create a new pile */
  card *pile = (card *)malloc(sizeof(card) * pile_no);

  for (int pile_index = 0; pile_index < pile_no; pile_index++) {
    // Get a random index
    int rand_card = rand_no();

    // Find a card that is not on our board yet
    while (gb->used[rand_card]) {
      rand_card = rand_no();
    }

    /* Update pile, and our used array */
    *(pile + pile_index) = *(*(gb->deck + rand_card));
    gb->used[rand_card] = true;
  }

  return pile;
}

game_board *init_game_board() {
  game_board *gb = (game_board *)malloc(sizeof(game_board));
  gb->deck = make_all_cards();

  // Initialize seeding for rng, then make the piles
  srand(time(NULL));

  // Generate starting piles
  for (int pile_no = 1; pile_no < 8; pile_no++) {
    card *pile = make_pile(gb, pile_no);

    memcpy(gb->table[pile_no], pile, (sizeof(card) * pile_no));
  }

  return gb;
}

void test() {
  game_board *gb = init_game_board();
  print_table(gb);
}

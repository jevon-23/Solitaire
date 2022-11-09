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

void print_game_board(game_board *gb) {
  /* TODO: print the gameboard */
}

void print_table(game_board *gb) {
  printf("printing out the current table of the game\n");
  for (int pile_no = 0; pile_no < 8; pile_no++) {
    printf("pile_no: %d\n", pile_no);
    for (int i = 0; i < pile_no; i++) {
      print_card((gb->table[pile_no].cards + gb->table[pile_no].max_len - i - 1));
    }
  }
}

/* Insert the card on to the back of the pile */
void insert_pile(pile **p, card *c) {
    *((*p)->cards + (*p)->max_len - (*p)->len - 1) = *c;
    (*p)->len++;
}

int rand_no() { return rand() % DECK_SIZE; }

/* Pile_no between 1-7
 *
 */
pile *make_pile(game_board *gb, enum pile_type type, int pile_no) {
  if (pile_no < 0 || pile_no > 7) {
    error("Invalid pile length\n", -1);
    return NULL;
  }

  /* Create a new pile */
  pile *p = (pile *)malloc(sizeof(pile));
  if (p == NULL)
    error("make_pile", MALLOC_ERR);

  p->type = type;
  switch ((int)type) {
    case DECK_PILE:
        p->cards = (make_all_cards());
        p->len = 52;
        p->max_len = 52;
        break;

    case FOUNDATION_PILE:
      // Create a bunch of empty piles 
      p->len = 0;
      p->max_len = SUIT_SIZE;
      p->cards = (card *)malloc(sizeof(card) * SUIT_SIZE);
      break;

    case TABLE_PILE:
      p->len = 0;
      p->max_len = MAX_PILE_LEN;
      p->cards = (card *)malloc(sizeof(card) * MAX_PILE_LEN);

      for (int pile_index = 0; pile_index < pile_no; pile_index++) {
        // Get a random card that is not on the board yet
        int rand_card = rand_no();
        while (gb->used[rand_card]) {
          rand_card = rand_no();
        }
        /* Update pile, and our used array */
        insert_pile(&p, (gb->deck->cards + rand_card));
        // *(p->cards + pile_index) = *(gb->deck->cards + rand_card);
        gb->used[rand_card] = true;
      }
      break;
    }

  return p;
}

game_board *init_game_board() {
  game_board *gb = (game_board *)malloc(sizeof(game_board));
  gb->deck = make_pile(gb, DECK_PILE, 0);

  // Initialize seeding for rng, then make the piles
  srand(time(NULL));

  /* Generate foundation piles */
  for (int i = 0; i < 3; i++) {
    pile *found = make_pile(gb, FOUNDATION_PILE, 0);
    memcpy((gb->foundation + i), found, (sizeof(card)));
  }

  // Generate starting piles
  printf("finished foundational piles\n");
  for (int pile_no = 1; pile_no < 8; pile_no++) {
    pile *p = make_pile(gb, TABLE_PILE, pile_no);
    memcpy((gb->table + pile_no), p, sizeof(pile));
  }

  return gb;
}

void test() {
  game_board *gb = init_game_board();
  print_table(gb);
}

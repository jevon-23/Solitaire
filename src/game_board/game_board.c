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

void print_pile(pile *p, enum pile_type type) {
  printf("pile_len = %d\n", p->len);
  if (p->len == 0) {
    printf("_\n");
  }
  for (int i = p->len - 1; i >= 0; i--) {
    print_card(p->cards + i);
  }
}

void print_foundational_piles(game_board *gb) {
  bool done = true;
  bool finished[4] = {false, false, false, false};

  // foundational piles
  for (int i = 0; i < SUIT_SIZE; i++) {
    for (int j = 0; j < 4; j++) {
      pile *found = (gb->foundation + j);
      if (finished[j])
        continue;
      if (found->len >= i) {
        printf("_");
        finished[j] = true;
        continue;
      }
      print_card(get_card(found, i));
    }
    printf("\n");

    for (int j = 0; j < 4; j++) {
      if (done && !finished[j])
        done = false;
    }
    if (done)
      break;
    else
      done = true;
  }
}

void print_table(game_board *gb) {
  bool done = true;
  bool finished[8] = {false, false, false, false, false, false, false, false};

  printf("printing out the current table of the game\n");
  for (int i = 0; i < SUIT_SIZE; i++) {
    for (int pile_no = 1; pile_no < 8; pile_no++) {
      if (finished[pile_no]) {
        printf("_:_|");
        continue;
      }
      pile *p = (gb->table + pile_no);
      if (p->len <= i) {
        printf("_:_|");
        finished[pile_no] = true;
        continue;
      }
      card *c = get_card(p, i);
      printf("%c:%c|",  card_value_to_str(c->val), card_suit_to_str(c->suit));
    }
    printf("\n");

    for (int j = 1; j < 8; j++) {
      if (done && !finished[j])
        done = false;
    }
    if (done)
      break;
    else
      done = true;
  }

}

void print_game_board(game_board *gb) {
  printf("Foundation: \n");
  print_foundational_piles(gb);
  print_table(gb);
}

card *get_card(pile *p, int index) {
  if (index >= p->len)
    return NULL;

  return (p->cards + p->max_len - p->len + index);
}

/* Insert the card on to the back of the pile */
void insert_pile(pile **p, card *c) {
  *((*p)->cards + (*p)->max_len - (*p)->len - 1) = *c;
  (*p)->len++;
}

int rand_no() { return rand() % DECK_SIZE; }

/* Pile_no between 1-7 */
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

  printf("Generating foundational piles\n");
  /* Generate foundation piles */
  for (int i = 0; i < 3; i++) {
    pile *found = make_pile(gb, FOUNDATION_PILE, 0);
    memcpy((gb->foundation + i), found, (sizeof(pile)));
  }
  printf("Finished generating foundational piles\n");

  printf("Generating starting piles\n");
  // Generate starting piles
  for (int pile_no = 1; pile_no < 8; pile_no++) {
    pile *p = make_pile(gb, TABLE_PILE, pile_no);
    memcpy((gb->table + pile_no), p, sizeof(pile));
  }
  printf("Finsihed generating starting piles\n");
  return gb;
}

void test() {
  game_board *gb = init_game_board();

  // print_card(get_card(gb->deck, 0));
  // print_pile(gb->deck, DECK_PILE);
  // print_pile((gb->foundation + 0));
  // print_foundational_piles(gb);
  // print_table(gb);
  print_game_board(gb);
}

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

/**************************/
/* Printing functionality */
/**************************/
void print_pile(pile *p, enum pile_type type) {
  printf("pile_len = %d\n", p->len);
  if (p->len == 0) {
    printf("_\n");
  }
  for (int i = p->len - 1; i >= 0; i--) {
    print_card(p->cards + i);
  }
}

bool check_finished(bool *finished, int len) {
    for (int j = 0; j < len; j++) {
      if (!finished[j])
          return false;
    }
    return true;
}

void print_stock_deck(game_board *gb) {
  int printed = 0;

  printf("printing left over deck\n");
  for (int i = 0; i < gb->stock->len; i++) {
    card *c = get_card(gb->stock, i);
    printf("%c:%c|", 
            card_value_to_str(c->val), card_suit_to_str(c->suit));
    printed++;
    if (printed % 5 == 0)
      printf("\n");
  }
    printf("\n");
}

void print_foundational_piles(game_board *gb) {
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

    if (check_finished(finished, 4))
      break;
  }
}

void print_table(game_board *gb) {
  bool finished[8] = {true, false, false, false, false, false, false, false};

  printf("printing out the current table of the game\n");
  for (int i = 0; i < SUIT_SIZE; i++) {
    for (int pile_no = 1; pile_no < 8; pile_no++) {
      if (finished[pile_no]) {
        printf("_:_|");
        continue;
      }
      pile *p = (gb->table + pile_no);
      if (p->len == i) {
        printf("_:_|");
        finished[pile_no] = true;
        continue;
      }
      card *c = get_card(p, p->len - i - 1);
      printf("%c:%c|",
              card_value_to_str(c->val),
              card_suit_to_str(c->suit));
    }
    printf("\n");

    if (check_finished(finished, 8)) 
        break;
  }
}

void print_game_board(game_board *gb) {
  printf("Foundation: \n");
  print_foundational_piles(gb);
  print_table(gb);
  print_stock_deck(gb);
}

/**********************/
/* Pile functionality */
/**********************/
card *get_card(pile *p, int index) {
  if (index >= p->len)
    return NULL;

  return (p->cards + p->max_len - p->len + index);
}

/* Insert the card on to the front of the pile */
void insert_pile(pile **p, card *c) {
  *((*p)->cards + (*p)->max_len - (*p)->len - 1) = *c;
  (*p)->len++;
}

int rand_no(max) { return rand() % max; }

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

    srand(time(NULL));
    for (int pile_index = 0; pile_index < pile_no; pile_index++) {
      // Get a random card that is not on the board yet
      int rand_card = rand_no(DECK_SIZE);
      while (gb->used[rand_card]) {
        rand_card = rand_no(DECK_SIZE);
      }

      /* Update pile, and our used array */
      card *c = (gb->deck->cards + rand_card);
      if (pile_index == pile_no-1) {
          c->hidden = false;
          print_card(c);
      }
      insert_pile(&p, c);
      gb->used[rand_card] = true;


    }
    break;

  case STOCK_PILE:;
    int *indicies = (int *)malloc(sizeof(int) * MAX_STOCK_LEN);
    int i_len = 0;
    p->cards = (card *)malloc(sizeof(card) * MAX_STOCK_LEN);
    p->max_len = MAX_STOCK_LEN;
    /* Get indicies of unused cards */
    for (int i = 0; i < 52; i++) {
        if (!gb->used[i]) {
            *(indicies + (i_len++)) = i;
        }
    }

    /* Shuffle the unused inidices and put the cards in unused */
    bool *check = (bool *)calloc(MAX_STOCK_LEN, sizeof(bool));
    for (int i = 0; i < i_len; i++) {
        int rand_card = rand_no(MAX_STOCK_LEN);
        while (check[rand_card]) {
            rand_card = rand_no(MAX_STOCK_LEN);
        }
        card *c = (gb->deck->cards + *(indicies + rand_card));
        insert_pile(&p, c);
    }

    free(indicies);
    break;

    default:
      error("Invalid pile passsed in to make_pile", -1);
      exit(-1);
  }
  return p;
}


/***************************/
/* Gameboard functionality */
/***************************/
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

  printf("Generating stock pile\n");
  gb->stock = make_pile(gb, STOCK_PILE, 0);
  printf("Finished generating stock pile\n");

  

  /* Generate the stock cards pile */
  return gb;
}

card *draw_one(game_board *gb) {
    card *c = get_card(gb->stock, gb->stock->index);
    if (c == NULL) {
        gb->stock->index = 0;
        c = get_card(gb->stock, gb->stock->index);
        if (c == NULL) {
            error("Couldn't get card out of deck or no more cards left", -1);
        }
    }
    gb->stock->index++;
    return c;
}

void test() {
  game_board *gb = init_game_board();
  print_game_board(gb);

  for (int i = 1; i < 8; i++) {

    pile *p = (gb->table + i);
    card *c = get_card(p, 0);
    print_card(c);
  }

  // for (int i = 0; i < 25; i++) {

  //   card *c = draw_one(gb);
  //   print_card(c);
  // }
}

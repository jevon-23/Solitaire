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

void print_stock_deck(game_board *gb, enum pile_type rec) {
  int printed = 0;

  pile *p = gb->stock;
  if (rec == STOCK_PILE_REC) {
      p = gb->stock_recycle;
      printf("printing recycle deck\n");
  } else {
    printf("printing left over deck\n");

  }

  for (int i = 0; i < p->len; i++) {
    card *c = pile_get_card(p, i);
    printf("%c:%c|", 
            card_value_to_str(c->val), card_suit_to_str(c->suit));
    printed++;
    if (printed % 5 == 0)
      printf("\n");
  }
    printf("\n");
}

void print_foundational_piles(game_board *gb) {
  printf("Foundation: \n");
  bool finished[4] = {false, false, false, false};

  // foundational piles
  for (int i = 0; i < SUIT_SIZE; i++) {
    for (int j = 0; j < 4; j++) {
      pile *found = (gb->foundation + j);
      if (finished[j])
        continue;
      if (found->len <= i) {
        printf("_:_|");
        finished[j] = true;
        continue;
      }
      card *c = pile_get_card(found, i);
    printf("%c:%c|", 
            card_value_to_str(c->val), card_suit_to_str(c->suit));
    }
    printf("\n");

    if (check_finished(finished, 4))
      break;
  }
}

void print_table(game_board *gb) {
  bool finished[8] = {true, false, false, false, false, false, false, false};

  printf("printing out the current table of the game\n");
  printf(" 1 | 2 | 3 | 4 | 5 | 6 | 7 |\n");
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
      card *c = pile_get_card(p, p->len - i - 1);
      if (!c->hidden )
      printf("%c:%c|",
              card_value_to_str(c->val),
              card_suit_to_str(c->suit));
      else printf("*:*|");
    }
    printf("\n");

    if (check_finished(finished, 8)) 
        break;
  }
}

void print_stock_top(game_board *gb) {
    printf("Top of the stock: \n");
    card *c = gb->top_stock_card;
    if (c == NULL) {
      printf("_:_|\n");
      return;
    }
    printf("%c:%c|\n",
            card_value_to_str(c->val),
            card_suit_to_str(c->suit));
}

void print_game_board(game_board *gb) {
  print_foundational_piles(gb);
  print_stock_top(gb);
  print_table(gb);
  print_stock_deck(gb, gb->stock->type);
  print_stock_deck(gb, gb->stock_recycle->type);
}

/**********************/
/* Pile functionality */
/**********************/
/* Get the card @ INDEX in pile */
card *pile_get_card(pile *p, int index) {
  if (index >= p->len)
    return NULL;
  return (p->cards + p->max_len - p->len + index);
}

/* Get the index of card in pile */
int pile_index_of(pile *p, card *c) {
    for (int i = 0; i < p->len; i++) {
        card *c2 = pile_get_card(p, i);
        if (c->val == c2->val && c->suit == c2->suit)
            return i;
    }
    return -1;
}

/* Pop off n amount of cards from the beginning of pile */
card *pile_pop_upto(pile *p, int n) {
    if (n == 0) {
        error("Cannot pop 0 values\n", -1);
        return NULL;
    } else if (p->len == 0)
        return NULL;
    else if (n > p->len) {
        error("Trying to pop more than whats left in pile\n", -1);
        return NULL;
    }

    card *out = (card *)malloc(sizeof(card) * n);

    for (int i = 0; i < n; i++) {
        card *c = pile_get_card(p, 0);
        p->len--;
        memcpy((out + i), c, sizeof(card));
        printf("\n");
    }

    card *next_card = pile_get_card(p, 0);

    if (next_card != NULL && next_card->hidden)
        next_card->hidden = false;
    return out;
}

/* Insert the card on to the front of the pile */
void insert_pile(pile **p, card *c) {
  // print_card(c);
  *((*p)->cards + (*p)->max_len - (*p)->len - 1) = *c;
  (*p)->len++;
}

/* Get random number up to MAX */
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
          // print_card(c);
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

    case STOCK_PILE_REC:
      p->cards = (card *)malloc(sizeof(card) * MAX_STOCK_LEN);
      p->len = 0;
      p->max_len = MAX_STOCK_LEN;
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

/* Initialize the game board */
game_board *init_game_board() {
  game_board *gb = (game_board *)malloc(sizeof(game_board));
  gb->deck = make_pile(gb, DECK_PILE, 0);
  card *e = make_card(empty, _E);
  gb->top_stock_card = e;


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

  /* Generate the stock cards pile */
  printf("Generating stock pile\n");
  gb->stock = make_pile(gb, STOCK_PILE, 0);
  printf("Finished generating stock pile\n");

  printf("Generating stock recycle bin\n");
  gb->stock_recycle = make_pile(gb, STOCK_PILE_REC, 0);
  return gb;
}


/* When we run out of cards in our stock pike, recycle it */
bool recycle_deck(game_board *gb) {
    // Copy stock from recycle in reverse to get og back
    printf("inside of rec\n");

    for (int i = 0; i < gb->stock_recycle->len; i++) {
        card *c = pile_get_card(gb->stock_recycle, i);
        print_card(c);
        insert_pile(&gb->stock, c);
    }

    gb->stock_recycle->len = 0;
    return true;
}

/* Draw one from the stock pyle, or recycle the deck completely and
 * return top card */
card *draw_one(game_board *gb) {
    card *c = pile_pop_upto(gb->stock, 1);
    if (c == NULL) {
        gb->stock->index = 0;
        c = pile_get_card(gb->stock, gb->stock->index);
        if (c == NULL) {
            error("Couldn't get card out of deck or no more cards left", -1);
          if (!recycle_deck(gb))
                  return NULL;
          c = pile_get_card(gb->stock, 0);
        }
    }
    gb->top_stock_card = c;
    insert_pile(&gb->stock_recycle, c);
    return c;
}

/* Transfer NUM_TRANSFER cards from SRC to DST */
bool transfer_pile(pile *dst, pile *src, int num_transfer) {
    if (num_transfer > (src)->len || (dst)->max_len < (dst)->len + num_transfer) 
        return false;

    card *dst_transfer_card = pile_get_card(dst, 0);
    card *src_receiver_card = pile_get_card(src, num_transfer-1);

    if (src_receiver_card == NULL)
        return false;
    
    bool dst_present = dst_transfer_card != NULL;

    if ((dst_present) && (dst_transfer_card->hidden || src_receiver_card->hidden)) {
        error("Cannot move onto a card that is not hidden\n", -1);
        printf("dst card: "); 
        print_card(dst_transfer_card);
        printf("\nsrc card: ");
        print_card(src_receiver_card);
        printf("\n");
        return false;
    }

    if (dst_present && ((int)dst_transfer_card->val != (int) src_receiver_card->val + 1)) {
        error("Cannot move onto a card that is not in inc. order\n", -1);
        printf("dst card: \n"); 
        print_card(dst_transfer_card);
        printf("\nsrc card: ");
        print_card(src_receiver_card);
        printf("\n");

        // printf("src->val = %d, dst->val = %d, src")
        return false;
    }

    if (dst_present && dst_transfer_card->suit_color == src_receiver_card->suit_color) {
        error("Connot move onto a card that is not the opposite color \n", -1);
        return false;
    }
    
    card *popped_cards = pile_pop_upto(src, num_transfer);

    for (int i = num_transfer-1; i >= 0; i--) 
        insert_pile(&dst, (popped_cards + i));
    return true;
}

void test() {
  game_board *gb = init_game_board();
  print_game_board(gb);

  pile *p = (pile *)(gb->table + 3);
  pile *p2 = (pile *)gb->foundation;
  // pile *p2 = (pile *)(gb->table + 4);
  // card *c = pile_get_card(p2, 1);
  // c->hidden = false;
  // c = pile_get_card(p2, 2);
  // c->hidden = false;
  // print_game_board(gb);
  printf("about to transfer\n");
  transfer_pile(p2, p, 1);
  printf("finished transfer\n");
  print_game_board(gb);

  // for (int i = 1; i < 8; i++) {

  //   pile *p = (gb->table + i);
  //   for (int j = 0; j < 7; j++) {
  //     card *c = pile_get_card(p, j);
  //     if (c != NULL) 
  //       print_card(c);
  //   }
  //   printf("\n");
  // }

  // for (int i = 0; i < 25; i++) {

  //     printf("fraw\n");
  //   card *c = draw_one(gb);
  //   print_card(c);
  //   print_stock_deck(gb, gb->stock->type);
  //   print_stock_deck(gb, gb->stock_recycle->type);

  // }
}

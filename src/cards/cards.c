#include "../../include/cards.h"
#include "../../include/utils.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_card(card *c) {
    printf("c->str: %c ~ c->val: %d ~ c->suit: %d -> %c ~ c->suit_color: %c ~ c->hidden: %d\n", 
            c->str, c->val, c->suit, c->suit_str, c->suit_color, c->hidden);
}

char card_suit_to_color(enum suits suit) {
  switch ((int)suit) {
      case diamonds:
          return 'R';
      case hearts:
          return 'R';
      case clubs: 
          return 'B';
      case spades:
          return 'B';
      default:
          return 0x0;
  }
}

/* Maps a card_value VAL to a string representation */
char card_suit_to_str(enum suits suit) {
  switch ((int)suit) {
      case diamonds:
          return 'D';
      case hearts:
          return 'H';
      case clubs: 
          return 'C';
      case spades:
          return 'S';
      default:
          return 0x0;
  }
}

/* Maps a card_value VAL to a string representation */
char card_value_to_str(enum card_values val) {
  switch ((int)val) {
  case _A:
    return 'A';
  case _2:
    return '2';
  case _3:
    return '3';
  case _4:
    return '4';
  case _5:
    return '5';
  case _6:
    return '6';
  case _7:
    return '7';
  case _8:
    return '8';
  case _9:
    return '9';
  case _10:
    return '0';
  case _J:
    return 'J';
  case _Q:
    return 'Q';
  case _K:
    return 'K';
  default:
    return -1;
  }
}

/* Make a card struct using based on SUIT and VALUE
 * where SUIT is the card suit and VALUE is the value of the card */
card *make_card(enum suits suit, enum card_values value) {
  char card_value_string = card_value_to_str(value);
  if (card_value_string == -1) {
    error("Failed to convert card value to a string\n", -1);
    return NULL;
  }

  card *c = (card *)malloc(sizeof(card));
  if (c == NULL) {
    error(NULL, MALLOC_ERR);
    return NULL;
  }

  c->str = card_value_string;
  c->val = value+1; // enum starts @ 0, we want to start @ 1
  c->suit = suit;
  c->suit_str = card_suit_to_str(suit);
  c->suit_color = card_suit_to_color(suit);
  c->hidden = true;

  return c;
}

card **make_suit(enum suits suit) {
  card **deck = (card **)malloc(sizeof(card *) * SUIT_SIZE);
  if (deck == NULL) {
      error(NULL, MALLOC_ERR);
      return NULL;
  }

  for (int value = _A; value <= _K; value++) {
    *(deck + value) = make_card(suit, value);
  }

  return deck;
}

card **make_all_cards() {
  card **deck = (card **)malloc(sizeof(card *) * DECK_SIZE);
  if (deck == NULL) {
      error(NULL, MALLOC_ERR);
      return NULL;
  }

  for (int suit = diamonds, offset = 0; suit <= spades; suit++, offset++) {
      int off = offset * SUIT_SIZE;
      card **suit_deck = make_suit(suit);
      memcpy((deck + off), suit_deck, sizeof(card) * SUIT_SIZE);
  }

  // printf("no exit early\n");
  return deck;
}

void testing() {
  enum card_values cv;
  enum suits s;
  s = diamonds;
  cv = _5;

  card **deck = make_all_cards();

  for (int i = 0; i < 52; i++) {
    printf("i: %d, card->str: %c ~ card->val: %d ~ card->suit: %d\n",
           i, (*(deck + i))->str, (*(deck + i))->val, (*(deck + i))->suit);
  }
}

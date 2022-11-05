#include <stdbool.h>
#ifndef CARDS_H
#define CARDS_H

#define DECK_SIZE 52
#define SUIT_SIZE 13

enum card_values {
    _A, 
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
    _J,
    _Q,
    _K };

enum suits { diamonds, hearts, clubs, spades };

typedef struct card {
  char str;             // str repr of card
  enum card_values val; // enum value of card
  enum suits suit;      // suit of class
  char suit_str;
  char suit_color; 
  bool hidden;


} card;

void testing();

/* Creates a deck of all of the cards. Every card every suit */
card **make_all_cards();

/* Create a card for A-K of the given SUIT */
card **make_suit(enum suits suit);

/* Make a card struct using based on SUIT and VALUE
 * where SUIT is the card suit and VALUE is the value of the card */
card *make_card(enum suits suit, enum card_values value);

void print_card(card *c);
#endif

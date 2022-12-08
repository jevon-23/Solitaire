#include "../include/cards.h"
#include "../include/game_board.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUIT "q\n"
#define DRAW "d\n"
#define T_FOUNDATION "_f\0"
#define T_STOCK "_s\0"

enum move_type {
  _QUIT,
  _DRAW,
  _TRANSFER_PILE,
  _TRANSFER_FOUNDATION,
  _TRANSFER_STOCK
};
typedef struct move {
  char *og_input;
  enum move_type type;
  char *src_pile;
  char *dst_pile;
  int num_move;
  card *(*draw_fn)(game_board *);
  bool (*move_fn)(pile *, pile *, int);
  void *rv;

} move;

pile *input_pile_to_pile(game_board *gb, char *input_pile) {
  if (!strcmp("p0\0", input_pile))
      return gb->table;
  else if (!strcmp("p1\0", input_pile))
      return (gb->table + 1);
  else if (!strcmp("p2\0", input_pile))
      return (gb->table + 2);
  else if (!strcmp("p3\0", input_pile))
      return (gb->table + 3);
  else if (!strcmp("p4\0", input_pile))
      return (gb->table + 4);
  else if (!strcmp("p5\0", input_pile))
      return (gb->table + 5);
  else if (!strcmp("p6\0", input_pile))
      return (gb->table + 6);
  else if (!strcmp("p7\0", input_pile))
      return (gb->table + 7);
  else 
      return NULL;
}

move *process_input(char *input, size_t len) {
  move *m = (move *)malloc(sizeof(move));
  m->og_input = (char *)malloc(sizeof(char) * len);
  strncpy(m->og_input, input, len + 1);

  /* If we are draw or quit */
  if (!strcmp(input, QUIT)) {
    m->type = _QUIT;
    return m;
  } else if (!strcmp(input, DRAW)) {
    m->type = _DRAW;
    m->draw_fn = &draw_one;
    return m;
  }

  /* Otherwise, transferring piles */

  /* Parse the input */
  char *input_ptr = input;
  char input_parse[3][3];
  int input_parse_len = 0;
  for (char *token = strtok_r(input, " ", &input_ptr);
       token != NULL || input_parse_len < 3;
       token = strtok_r(NULL, " ", &input_ptr), input_parse_len++) {
    strncpy(*(input_parse + input_parse_len), token, strlen(token));
    input_parse[input_parse_len][2] = '\0';
  }

  m->src_pile = (char *)malloc(sizeof(char) * strlen(input_parse[0]));
  m->dst_pile = (char *)malloc(sizeof(char) * strlen(input_parse[2]));
  strncpy(m->src_pile, input_parse[0], strlen(input_parse[0]));
  m->num_move = atoi(input_parse[1]);
  strncpy(m->dst_pile, input_parse[2], strlen(input_parse[2]));

  /* Set the type of transfer we are doing */
  if (!strcmp(m->dst_pile, T_FOUNDATION)) {
      m->type = _TRANSFER_FOUNDATION;
  } else if (!strcmp(m->src_pile, T_STOCK)) {
      m->type = _TRANSFER_STOCK;
  } else {
    m->type = _TRANSFER_PILE;
  }
  printf("src: %s, dst: %s\n", m->src_pile, m->dst_pile);
  m->move_fn = &transfer_pile;

  return m;
}

void processCLI(int argc, char **argv) {
  if (argc != 1)
    exit(-1);
}

/* TODO: We need to make a front end for the everything
 *       Do we use sdl? js? If we want to use another
 *       lang, which is the point of this project, we need to
 *       make an api for interfacing from c -> lang.
 */

int main(int arg, char *argv[]) {
  processCLI(arg, argv);

  // testing();
  // test();

  game_board *gb = init_game_board();
  int num_plays = 0;
  while (true) {
    print_game_board(gb);

    /* Get the next input */
    char *input = NULL;
    size_t input_len = 0;
    input_len = getline(&input, &input_len, stdin);

    /* Go from input to a move */
    move *next_move = process_input(input, input_len);
    printf("back inside of main\n");

    switch ((int)next_move->type) {
    case _QUIT:
      printf("Game is over\n");
      return 0;
    case _DRAW:
      printf("Drawing a card\n");
      next_move->draw_fn(gb);
      break;
    case _TRANSFER_PILE:;
      pile *src = input_pile_to_pile(gb, next_move->src_pile);
      pile *dst = input_pile_to_pile(gb, next_move->dst_pile);
      next_move->move_fn(dst, src, next_move->num_move);

      break;
    default:
      printf("invalid move, try again\n");
      break;
    }

    num_plays++;
    free(next_move);
  }
  /* Make the game loop */
  printf("Successful build!\n");
  return 0;
}

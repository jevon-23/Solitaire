#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cards.h"
#include "../include/game_board.h"

int processCLI(int argc, char **argv, char **filename) {
  if (argc != 2) {
    printf("usage:");
    exit(-1);
  }
  return 0;
}
int main(int arg, char *argv[]) {
  // testing();
  test();
  printf("Successful build!\n");
  return 0;
}

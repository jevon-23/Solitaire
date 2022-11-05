#include "../../include/utils.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(char *err_str, int default_err) {

  switch (default_err) {
  case MALLOC_ERR:
    if (err_str != NULL) {
      printf("Error inside of: %s", err_str);
    }
    printf("Failed to malloc space\n");
    break;
  default:
    printf("Error was thrown: %s\n", err_str);
  }
}

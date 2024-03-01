#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get.h"

char* readline() {
  char buffer[81] = {0};
  char *my_string = NULL;
  int length = 0;
  int element = 0;
  do {
    int flag = 0;
    element = scanf("%80[^\n]%n", buffer, &flag);
    if (element < 0 || flag == EOF) {
      if (!my_string) {
        return NULL;
      } else if (flag == EOF) {
        break;
      }
    } else if (element > 0 && flag != EOF) {
      int chunk_len = strlen(buffer);
      int str_len = length + chunk_len;
      char* temp = realloc(my_string, str_len + 1);
      if (!temp) {
        free(my_string);
        return NULL;
      }
      my_string = temp;
      memcpy(my_string + length, buffer, chunk_len);
      length = str_len;
      my_string[str_len] = '\0';
    } else {
      scanf("%*[^\n]");
      scanf("%*c");
    }
  } while (element > 0);
    if (my_string) {
      char* temp = realloc(my_string, length + 1);
      if (!temp) {
        free(my_string);
        return NULL;
      }
      my_string = temp;
      my_string[length] = '\0';
    } else {
      my_string = calloc(1, sizeof(char));
    }
  return my_string;
}

int get_unsigned_int() {
  int number;
  int flag = scanf("%u", &number);
  if (flag == -1) {
    return -1;
  } 
  while ((flag != 1 || number <= 0) && flag != EOF) {
    printf("\nInvalid Input");
    printf("\nEnter Correct Unsigned Integer Number:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    flag = scanf("%u", &number);
    if (flag == -1) {
      return -1;
    }
  }
  scanf("%*[^\n]");
  scanf("%*c");
  return number;
}

int get_int() {
  int number;
  int flag = scanf("%d", &number);
  if (flag == -1) {
    return 0;
  }
  while ((flag != 1 || number < 0) && flag != EOF) {
    printf("\nInvalid Input");
    printf("\nEnter Correct Integer Number:\n");
    scanf("%*[^\n]");
    scanf("%*c");
    flag = scanf("%d", &number);
    if (flag == -1) {
      return 0;
    }
  }
  scanf("%*[^\n]");
  scanf("%*c");
  return number;
}

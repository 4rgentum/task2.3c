#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dialog.h"

int main() {
  Table *table = new_table();
  const char* msgs[] = {"0. Quit", "1. Insert", "2. Search", "3. Erase", "4. Print", "5. Load File"};
  const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
  int (*fptr[])(Table *) = {NULL, D_Insert, D_Search, D_Erase, D_Print, D_load};

  int rc;
  while (rc = dialog(msgs, NMsgs)) {
    if (!fptr[rc](table)){
      break;
    }
  }
  printf("End\n");
  clearTable(table);
  return 0;
}

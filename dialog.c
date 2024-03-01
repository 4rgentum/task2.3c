#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dialog.h"

int D_Insert(Table* table) {
  int rc;
  const char* errmsgs[] = {"Ok", "No Table"};
  printf("Input Key:\n");
  KeyType key = get_unsigned_int();
  if (key == -1){ 
    printf("End Of File\n");
    return 0; 
  }
  printf("Input Info:\n");
  InfoType *info = readline();
  if (info == NULL){
    free(info);
    printf("End Of File\n");
    return 0;
  }
  rc = insert(table, key, info);
  free(info);
  printf("\n");
  printf("%s: %u\n",errmsgs[rc], key);
  return 1;
}

int D_Erase(Table* table) {
  int rc;
  printf("Input Key:\n");
  KeyType key = get_unsigned_int();
  const char* errmsgs[] = {"Ok", "This Element Doesn't Exist", "Table Is Empty"};
  if (key == -1){ 
    printf("End Of File\n");
    return 0; 
  }
  printf("Do You Want Erase By Realese: 1 - If Yes, Another Number If No:\n");
  int fg = get_unsigned_int();
  if (fg == -1) {
    printf("End Of File");
    return 0;
  } else {
    int rc;
    if (fg == 1) {
      printf("Input Release:\n");
      RelType release = get_int();
      rc = erase(table, key, release);
    } else {
      rc = erase(table, key, 0);
    }
    printf("\n");
    printf("%s \n", errmsgs[rc]);
    return 1;
  }
}

int D_Search(Table *table) {
  const char* errmsgs[] = {"Ok", "This Element Doesn't Exist", "Table Is Empty"};
  printf("Input Key:\n");
  KeyType key = get_unsigned_int();
  if (key == -1){ 
    printf("End Of File\n");
    return 0; 
  }
  printf("Do You Want Search By Realese: 1 - If Yes, Another Number If No:\n");
  int fg = get_unsigned_int();
  Table *res = (Table *) calloc(1, sizeof(Table));
  res->msize = table->msize;
  res->csize = 0;
  res->ks = (KeySpaceElem **) malloc((res->msize) * sizeof(KeySpaceElem *));
  for (int i = 0; i < res->msize; i++) {
    res->ks[i] = new_key_space();
  }
  if (fg == -1) {
    printf("End Of File");
    clearTable(res);
    return 0;
  } else {
    int rc;
    if (fg == 1) {
      printf("Input Release:\n");
      RelType release = get_int();
      rc = search(table, key, res, release);
    } else {
      rc = search(table, key, res, 0);
    }
    if (!rc) {
      printTable(res);
    }
    printf("\n");
    printf("%s \n", errmsgs[rc]);
    clearTable(res);
    return 1;
  }
}

int D_Print(Table* table) {
  int rc;
  const char* errmsgs[]={"Ok", "Table Is Empty"};
  rc = printTable(table);
  printf("\n");
  printf("%s \n",errmsgs[rc]);
  return 1;
}

int D_load(Table* table) {
  const char* errmsgs[]={"Table Is Loaded", "File Can't Be Open"}; 
  printf("Input File Name:\n");
  char *name = readline();
  if (!name) {
    printf("End");
    return 1;
  }
  int rc = load(&name, table);
  printf("\n");
  printf("%s %s\n", errmsgs[rc], name);
  free(name);
  return 1;
}

int dialog(const char* msgs[], int flag) {
  char* errmsg = "";
  int rc;
  do {
    puts(errmsg);
    errmsg = "Invalid Input, Repeat\n";
    for (int i = 0; i < flag; i++) {
      puts(msgs[i]);
    }
    puts("Enter:\n");
    rc = get_int();
    printf("\n");
    
  } while (rc < 0 || rc >= flag);
  return rc;
}


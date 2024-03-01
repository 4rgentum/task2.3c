#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "get.h"

unsigned int hash(unsigned int key, unsigned int table_size) {
    unsigned int hash_value = 5381;  // Начальное значение хеша
    // Проход по каждому байту ключа
    unsigned char* key_ptr = (unsigned char*)&key;
    for (size_t i = 0; i < sizeof(unsigned int); i++) {
        hash_value = ((hash_value << 5) + hash_value) + key_ptr[i];
    }
    // Взятие остатка от деления на размер таблицы
    return hash_value % table_size;
}

Item *new_item() {
  Item *elem = (Item *) malloc(sizeof(Item));
  elem->info = NULL;
  return elem;
}

KeySpaceElem *new_key_space() {
  KeySpaceElem *ks = (KeySpaceElem *) malloc(sizeof(KeySpaceElem));
  ks->key = 0;
  ks->release = 0;
  ks->info = new_item();
  ks->next = NULL;
  return ks;
}

Table *new_table() {
  Table *table = (Table *) calloc(1, sizeof(Table));
  printf("Input Max Size of Table:\n");
  table->msize = get_int();
  if (!table->msize) {
    printf("End Of File\n");
    free(table);
    return NULL;
  }
  table->csize = 0;
  table->ks = (KeySpaceElem **) malloc((table->msize) * sizeof(KeySpaceElem *));
  for (int i = 0; i < table->msize; i++) {
    table->ks[i] = new_key_space();
  }
  return table;
}

int insert(Table* table, KeyType key, char* info) {
  if (table == NULL || table->ks == NULL) {
    return 1;
  }
  IndexType index = hash(key, table->msize);
  KeySpaceElem* curr = table->ks[index];
  RelType count_rel = 0;
  while (curr != NULL) {
    if (curr->key == key) {
      count_rel++;
    }
    curr = curr->next;
  }
  
  KeySpaceElem* elem = new_key_space();
  elem->key = key;
  elem->release = count_rel + 1;
  elem->info->info = strdup(info);
  elem->next = NULL;
  
  elem->next = table->ks[index];
  table->ks[index] = elem;
  (table->csize)++;
  return 0;
}

int load(char **name, Table* table) {
  FILE *file = fopen(*name, "r");
  if (file == NULL) {
    return 1;
  }
  KeyType key;
  KeyType par;
  InfoType* info;
  int count = 0;
  while (fscanf(file, "%u\n%u\n%s\n", &key, &par, info) == 3) {
    insert(table, key, info);
    count++;
  }
  if (!count) {
    fclose(file);
    return 1;
  }
  fclose(file);
  return 0;
}

int erase(Table* table, KeyType key, RelType release) {
  if (table == NULL || table->ks == NULL) {
    return 2;
  }
  IndexType index = hash(key, table->msize);
  KeySpaceElem* curr = table->ks[index];
  KeySpaceElem* prev = NULL;
  int flag = 0;
  if (release == 0) {
    while (curr != NULL) {
      if (curr->key == key) {
        if (prev == NULL) {
          table->ks[index] = curr->next;
          free(curr->info->info);
          free(curr->info);
          free(curr);
          curr = table->ks[index];
        } else {
          prev->next = curr->next;
          free(curr->info->info);
          free(curr->info);
          free(curr);
          curr = prev->next;
        }
        flag = 1;
        (table->csize)--;
      } else {
        prev = curr;
        curr = curr->next;
      }
    }
  } else {
    while (curr != NULL) {
      if (curr->key == key && curr->release == release) {
        flag = 1;
        if (prev == NULL) {
          table->ks[index] = curr->next;
        } else {
          prev->next = curr->next;
        }
        free(curr->info->info);
        free(curr->info);
        free(curr);
        (table->csize)--;
        break;
      }
      prev = curr;
      curr = curr->next;
    }
  }
  if (!flag) {
    return 1;
  } else {
    return 0;
  }
}

int search(Table *table, KeyType key, Table *res, int flag) { //flag = 0 by key, flag != 0 by rel => flag = rel
  if (table == NULL || table->ks == NULL) {
    return 2;
  }
  IndexType index = hash(key, table->msize);
  KeySpaceElem* curr = table->ks[index];
  int f = 0;
  while (curr != NULL) {
    if (flag != 0) {
      if (curr->key == key && curr->release == flag) {
        f = 1;
        insert(res, key, curr->info->info);
        for (int i = 0; i < res->msize; i++) {
          if (res->ks[i] != NULL) {
            res->ks[i]->release = flag;
          }
        }
        break;
      }
    } else {
      if (curr->key == key) {
        f = 1;
        insert(res, key, curr->info->info);
      }
    }
    curr = curr->next;
  }
  if (f == 0) {
    return 1;
  } else {
    return 0;
  }
}

int printTable(Table *table) {
  if (table == NULL || table->ks == NULL || table->csize == 0) {
    return 1;
  }
  for (int i = 0; i < table->msize; i++) {
    KeySpaceElem *curr = table->ks[i];
    while (curr != NULL) {
      if (curr->release > 0) {
        printf("Key: %u, Realese: %d, Info: %s\n", curr->key, curr->release, curr->info->info);
      }
      curr = curr->next;
    }
    free(curr);
  }
  return 0;
}

int clearTable(Table *table) {
  if (table == NULL) {
    return 1;
  }
  if (table->ks == NULL) {
    free(table);
    return 1;
  }
  for (int i = 0; i < table->msize; i++) {
    if (table->ks[i] != NULL) {
      KeySpaceElem *curr = table->ks[i];
      KeySpaceElem *next = NULL;
      while (curr != NULL) {
        next = curr->next;
        free(curr->info->info);
        free(curr->info);
        free(curr);
        curr = next;
      }
      table->ks[i] = NULL;
    }
  }
  free(table->ks);
  free(table);
  return 0;
}

#ifndef TABLE_H
#define TABLE_H

typedef unsigned int KeyType;
typedef unsigned int IndexType;
typedef int RelType;
typedef char InfoType;

typedef struct Item {
  InfoType *info;
} Item;

typedef struct KeySpaceElem {
  KeyType key;
  RelType release;
  Item *info;
  struct KeySpaceElem *next;
} KeySpaceElem;

typedef struct Table {
  KeySpaceElem **ks;
  IndexType msize;
  IndexType csize;
} Table;

unsigned int hash(unsigned int key, unsigned int table_size);
Item *new_item();
KeySpaceElem *new_key_space();
Table *new_table();
int insert(Table* table, KeyType key, char* info);
int load(char **name, Table* table);
int erase(Table* table, KeyType key, RelType release);
int search(Table *table, KeyType key, Table *res, int flag);
int printTable(Table *table);
int clearTable(Table *table);

#endif

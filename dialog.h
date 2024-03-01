#ifndef DIALOG_H
#define DIALOG_H

#include "get.h"
#include "table.h"

int D_load(Table* table);
int D_Insert(Table *table);
int D_Erase(Table *table);
int D_Search(Table *table);
int D_Print(Table *table);
int dialog(const char* msgs[], int flag);

#endif

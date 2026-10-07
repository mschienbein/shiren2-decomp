#include "common.h"

/* Prefix view: the holder's first word is its item-list pointer. */
typedef struct ListHolder {
    void *list;
} ListHolder;

void func_800A1888(ListHolder *holder, void *list) {
    holder->list = list;
}

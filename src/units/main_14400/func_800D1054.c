#include "common.h"
typedef struct List List;
/* Fills three content-list pointer slots (stores at +0/+4/+8). */
extern void func_800D10F4(List **slots);
extern void *func_800CDA70(List *list, unsigned char kind);
s32 func_800D1054(s32 arg0) {
    List *entries[3];
    s32 i;
    func_800D10F4(entries);
    i = 0;
    for (;;) {
        if (entries[i]) {
            void *found = func_800CDA70(entries[i], (unsigned char)arg0);
            if (found) return 1;
        }
        i++;
        if (i >= 3) {
            return 0;
        }
    }
}

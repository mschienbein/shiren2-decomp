#include "common.h"
typedef struct Entry { unsigned char kind_00; unsigned char pad_01[3]; s32 id_04; } Entry;
extern void func_8012C690(s32 id, u32 mask, u32 bits);
extern s32 func_8012CEC8(s32 index);
void func_8012ABAC(Entry *entry) {
    switch (entry->kind_00) {
    case 0: func_8012C690(entry->id_04, -2, 1); break;
    case 1: func_8012C690(entry->id_04, -2, 0); break;
    case 2: func_8012CEC8(entry->id_04); break;
    }
}

#include "common.h"
typedef struct { unsigned char fields[0x78]; } EntryB;
extern void func_8008C950(EntryB *entry);
void func_8008C9A0(EntryB *entries, u32 count) {
    u32 i;
    for (i = 0; i < count; i++) func_8008C950(&entries[i]);
}

#include "common.h"

typedef struct {
    void (*handler)(void *);
    unsigned short unk4;
    unsigned char pad6[0xE];
    s32 unk14;
    unsigned char pad18[0x5C];
} Entry;

extern Entry D_801BA380[];
extern s32 func_80084CD4(void (*handler)(void *));

void *func_80085154(void (*handler)(void *), s32 value) {
    Entry *entry = &D_801BA380[func_80084CD4(handler)];

    entry->unk14 = value;
    if (entry->unk4 == 0) {
        entry->unk4 = 1;
    }
    return entry;
}

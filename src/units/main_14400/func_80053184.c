#include "common.h"
typedef short s16;
typedef unsigned char u8;
/* 12-byte fade record: halfword delta at +4 and bytes +6..+9 are untouched here. */
typedef struct { s16 *unk0; u8 pad4[8]; } Entry;
extern Entry D_801616D0[];
extern s16 *func_80052F54(s16);
extern u8 func_800535C8(Entry *);
extern void func_800535D0(s32, Entry *, s32, u8);
void func_80053184(s16 arg0, u8 arg1, u8 arg2) {
    Entry *entry = D_801616D0;
    s32 i;
    for (i = 0; i < 2; i++, entry++) {
        if (!func_800535C8(entry)) {
            entry->unk0 = func_80052F54(arg0);
            break;
        }
        if (*entry->unk0 == arg0 || i > 0) break;
    }
    func_800535D0(2, entry, arg1, arg2);
}

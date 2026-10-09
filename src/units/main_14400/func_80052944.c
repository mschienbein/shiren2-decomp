#include "common.h"

typedef unsigned char u8;
typedef struct { signed char x, y; } Pair;
typedef struct { s32 handle; Pair position; } Playback;
/* Complete 0xC-byte sound state: type +0, playback.handle +4, playback.position +8/+9. */
typedef struct { short type; Playback playback; } State;
/* Eight-byte ROM range record (see func_80052914). */
typedef struct { u32 start; u32 end; } Range;

extern State D_80161644;
extern u8 D_801397D9, D_8016166E, D_8016166F;
extern Range D_8014B5A4[];
extern const u8 D_8014B894[];
extern void *D_801D9354;
void func_800526B0(u8 a, u8 b);
void func_80052914(s32 index, void *dst, Range *ranges);
s32 func_80052AD8(void *bank);
s32 func_80129EE0(void *bank);
s32 func_80129F10(void *bank, s32 arg);
void func_80052B50(s32 handle);
void func_80052CF8(void);
void func_800533B8(void);
s32 func_80052D08(short type);
s32 func_8012A4E4(s32 id);
s32 func_8012A534(s32 handle, s32 volume);

/* Starts background music track `type` unless it is already playing. */
void func_80052944(short type) {
    void *bank = D_801D9354;

    if (type == D_80161644.type && (unsigned short)(type - 0x46) >= 2U && func_8012A4E4(D_80161644.playback.handle)) return;
    if (D_8016166E) {
        if (!func_80052D08(type)) D_80161644.type = type;
    } else {
        D_80161644.type = type;
        func_80052B50(D_80161644.playback.handle);
        func_80052CF8();
        func_800533B8();
        func_80052914(D_80161644.type, D_801D9354, D_8014B5A4);
        switch (D_80161644.type) {
        case 36:
        case 38:
            D_80161644.playback.handle = func_80052AD8(bank);
            break;
        case 9:
        case 49:
        case 57:
            D_80161644.playback.handle = func_80129F10(bank, 0);
            break;
        default:
            D_80161644.playback.handle = func_80129EE0(bank);
            break;
        }
        D_80161644.playback.position.x = D_8014B894[D_80161644.type];
        func_8012A534(D_80161644.playback.handle, (u8)D_80161644.playback.position.x);
        if (D_8016166F == 1) func_800526B0(D_801397D9, 0);
    }
}

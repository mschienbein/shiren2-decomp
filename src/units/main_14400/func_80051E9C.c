#include "common.h"
typedef struct { signed char x; signed char y; } Position;
typedef struct { short index; unsigned char count; unsigned char flags; } Mapping;
typedef struct { s32 handle; Position position; } Playback;
/* Complete 0xC-byte sound state (type +0, playback.handle +4, playback.position
 * +8/+9), the canonical layout of D_80161644/D_80161650; D_801397CC is the
 * saved previous-stream copy of D_8016165C. */
typedef struct { short type; Playback playback; } State;
extern unsigned char D_8016166E, D_8016166F;
extern Mapping D_8014B8F0[];
extern void *D_801DEA08;
unsigned char D_801397D9 = 0x50;
unsigned char D_801397DA = 0x06;
extern State D_8016165C, D_801397CC;
extern s32 D_80161674[4];
extern void func_8012A99C(void *object);
extern s32 func_8012A4E4(s32 object);
extern s32 func_8012A434(s32 object, s32 flag);
extern void func_800526B0(unsigned char x, unsigned char y);
extern s32 func_8012A3BC(s32 kind);
extern s32 func_80052B6C(s32 object);
extern s32 func_8012A1DC(s32 index, s32 x, s32 y, s32 flag, s32 mode);
extern s32 func_80052EC4(short index);
extern void func_80053438(s32 index);
extern void *func_80052EE8(short index, s32 object, Position position);
/* ODD_C: These inline accessors keep the real mapping/state/position views
 * while preserving the original load widths and call scheduling. */
static inline short map_index(short index) {
    return D_8014B8F0[index].index;
}
/* ODD_C: the field-address accessor keeps the handle address in s0 across
 * the playback calls and its load in the first jal delay slot. A direct
 * &D_801397CC.playback.handle rematerializes the address and adds 4 bytes. */
static inline s32 *previous_handle(void) {
    return &D_801397CC.playback.handle;
}
static inline void update_previous(s32 *handle) {
    if (func_8012A4E4(*handle)) func_8012A434(*handle, 1);
    else func_800526B0(D_801397D9, D_801397DA);
}
/* ODD_C: the unsigned-byte setter keeps 255 in the original li operands;
 * assigning position.x directly instead produces -1 at two instructions. */
static inline void set_x(Position *position, unsigned char x) {
    position->x = x;
}
void func_80051E9C(short index, Position position) {
    s32 selected = 0;
    s32 repeat = 1;
    short original = index;
    s32 mapped;
    s32 wait_needed;
    s32 i;
    s32 count;
    if (D_8016166E || index == -1) return;
    {
        s32 value = map_index(index);
        mapped = (short)value;
        /* ODD_C: group bank selection with the optional previous-stream
         * transition; ordinary effects break before that transition. The block
         * also preserves the original copied-index comparison. */
        do {
            if (value >= 0x182) {
                func_8012A99C(D_801DEA08);
                mapped -= 0x182;
            }
            if ((unsigned short)(index - 0x1A2) >= 2) break;
            update_previous(previous_handle());
            D_8016166F = 1;
        } while (0);
    }
    switch (original) {
        case 0x20:
        case 0x13F:
        case 0x14A:
        case 0x17B:
        case 0x17C:
        case 0x183:
            repeat = 0;
            break;
        case 0x145:
        case 0x146:
        case 0x15A:
        case 0x17E:
        case 0x17F:
            repeat = 0;
            break;
        case 0x8A:
        case 0xA6:
            set_x(&position, 255);
            break;
    }
    {
        s32 available = func_8012A3BC(3);
        wait_needed = 0;
        if (D_8014B8F0[original].count > 24 - available) {
            if (original == D_8016165C.type) wait_needed = !repeat;
            else wait_needed = 1;
        }
    }
    for (i = 0; i < 4; i++) {
        if (D_80161674[i]) {
            if (!func_8012A4E4(D_80161674[i])) D_80161674[i] = 0;
            else if (!selected) {
                selected = D_80161674[i];
                D_80161674[i] = 0;
            }
        }
    }
    if (wait_needed) {
        func_80052B6C(selected);
        do {
            s32 available = func_8012A3BC(3);
            if (D_8014B8F0[original].count <= 24 - available) break;
        } while (1);
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        s32 handle = D_80161674[i];
        if (handle) D_80161674[count++] = handle;
    }
    if (count >= 4) count = 3;
    D_8016165C.type = original;
    D_8016165C.playback.position = position;
    D_8016165C.playback.handle = func_8012A1DC((short)mapped, (unsigned char)position.x, (unsigned char)position.y, repeat, -1);
    if (func_80052EC4(D_8016165C.type)) {
        func_80053438(D_8016165C.type);
        func_80052EE8(D_8016165C.type, D_8016165C.playback.handle, D_8016165C.playback.position);
    } else D_80161674[count] = D_8016165C.playback.handle;
    if ((u32)((unsigned short)D_8016165C.type - 0x1A2) < 2) D_801397CC = D_8016165C;
}

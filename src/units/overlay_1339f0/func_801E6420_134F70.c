#include "func_801E6420_134F70/types.h"

/* OvlSaveInfo (the opaque 24-byte save-slot summary) comes from the overlay slice header shared
 * with the neighbouring units; ROM 0x134FFC..0x1350B8 copies it from save+0xA. */
extern u8 *func_801E4F20_133A70(s32 slot, s32 *message);
extern void func_801E63C4_134F14(void);
extern void func_80048380(void);
extern void func_80048240(u16 id, ...);
extern void func_80049BF0(s32 mode);
extern s32 func_80049CB4(s32 id, ...);
extern s32 D_8013960C;
/* Signed like main func_8009D068.c (extern s32 D_801E7898[]), which copies the word. */
extern s32 D_801E7898_1363E8[3];
extern OvlSaveInfo D_801E78A4_1363F4[3];

/* Big-endian word from four bytes; ROM 0x1350C4..0x1350E8. */
static inline u32 read_be32(u8 *p) {
    return (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];
}

/* func_801E63E8_134F38 forwards this result to main func_8009D068:
 * 0 = empty slot, 2 = read error (message shown), 3 = summary loaded. */
s32 func_801E6420_134F70(u8 slot) {
    s32 message;
    u8 *cursor;
    u8 *save;

    cursor = func_801E4F20_133A70(slot, &message);
    if (cursor == 0) {
        if (message == -1) {
            return 0;
        }
        func_80048380();
        func_80048240((u16)message);
        func_80049BF0(0);
        func_80049CB4(2);
        func_801E63C4_134F14();
        D_8013960C = 0;
        func_80048380();
        return 2;
    }
    /* The buffer base is kept (ROM 0x801E64A8 copies it) while the cursor
     * skips the 0xA-byte header for the summary copy; the trailing word is
     * then read from base+0x27F8 (ROM 0x801E656C/0x801E6574). */
    save = cursor;
    cursor += 0xA;
    D_801E78A4_1363F4[slot] = *(OvlSaveInfo *)cursor;
    cursor = save + 0x27F8;
    D_801E7898_1363E8[slot] = read_be32(cursor);
    return 3;
}

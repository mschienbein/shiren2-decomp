#include "common.h"

/* Prefix of the record object at 0x80147680; valueA is its signed +0xA member,
 * not an independently allocated object at the interior splat label. */
typedef struct {
    unsigned char pad0[9];
    unsigned char flags9;
    signed char valueA;
    unsigned char padB[0x11];
    void *child1C;
} RecordObject;
extern RecordObject D_80147680;
void func_80049C90(s32 mode, s32 handle);
s32 func_80049CB4(s32 id, ...);
s32 func_800A08D8(s32 mode, s32 key, s32 sel) {
    s32 se;
    switch (D_80147680.valueA) {
    case 0:
        return 0;
    case 1:
        if (key == -2) return 0;
        se = 0;
        switch (sel) {
        case 0: se = 6; break;
        case 1: se = 2; break;
        case 2: se = 5; break;
        }
        func_80049CB4(0x129, se);
        return 0;
    case 2:
        if (sel == 0) {
            func_80049C90(mode, key);
            return 1;
        }
        if (key == -2) return 0;
        se = 0;
        switch (sel) {
        case 1: se = 0xE; break;
        case 2: se = 0xC; break;
        }
        func_80049CB4(0x129, se);
        return 0;
    }
    return 0;
}

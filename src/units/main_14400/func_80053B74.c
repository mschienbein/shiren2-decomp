#include "common.h"
typedef struct {
    s32 field_0, field_4, field_8, field_C, field_10, field_14, field_18, field_1C;
    char messages_20[4][0x100];
} Slot;
/* func_800538E0 initializes one 0x420-byte slot ending at 0x80161B44. */
extern Slot D_80161724[1];
/* Plain window draw callback: registered through func_80081D60 with a null context, so the
 * window dispatcher func_800833B8 calls it with no argument (0x8008342C); its body reads no
 * argument register. */
extern void func_80054258(void);
extern s32 func_80081D60(s32, s32, s32, s32, void (*)(void));
extern void func_80082658(u32, s32);
extern void func_800835AC(u32);
extern s32 func_8005ECF8(char *, const char *, char *);
s32 func_80053B74(s32 index, s32 a, s32 b, s32 c, s32 d, const char *fmt, char *args) {
    Slot *slot = &D_80161724[index];
    s32 result;
    if (slot->field_0 == -1) {
        slot->field_0 = func_80081D60(a, b, c, d, func_80054258);
        func_80082658(slot->field_0, 1);
        result = 0;
        slot->field_18 = 0;
        slot->field_14 = 0;
        slot->field_4 = 0;
    } else {
        if (!slot->field_8) {
            s32 next = (slot->field_14 + 1) % 4;
            if (next == slot->field_18) return -1;
            slot->field_14 = next;
        } else {
            slot->field_18 = 0;
            slot->field_14 = 0;
            slot->field_4 = 0;
        }
        result = -1;
        func_800835AC(slot->field_0);
    }
    slot->field_8 = 0;
    slot->field_C = 0x78;
    func_8005ECF8(slot->messages_20[slot->field_14], fmt, args);
    return result;
}

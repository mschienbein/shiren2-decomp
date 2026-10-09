#include "common.h"
typedef struct { s32 cursor; s32 category; } Iter800C878C;
typedef struct { unsigned char pad_00[0x1E]; unsigned char flags_1E; } Obj800C878C;
extern unsigned short D_8014767C;
extern s32 func_80046240(void);
extern s32 func_800A8FC8(Iter800C878C *iter, s32 kind);
extern void *func_800A910C(Iter800C878C *iter);
extern unsigned short func_800E08B0(void *obj);
/* Original call at 800C8838 explicitly forwards a signed 16-bit turns value. */
extern void func_800C8D78(Obj800C878C *obj, short turns);
static __inline__ s32 restricted(void) {
    s32 result = 0;
    if (func_80046240() || ((D_8014767C >> 6) & 1)) result = 1;
    return result;
}
static __inline__ s32 player_flag(Obj800C878C *obj) {
    return (obj->flags_1E >> 2) & 1;
}
static __inline__ s32 accepts_update(Obj800C878C *obj) {
    s32 result = 0;
    if (func_800E08B0(obj)) result = !player_flag(obj);
    return result;
}
static __inline__ Obj800C878C *next_object(Iter800C878C *cursor) {
    return func_800A910C(cursor);
}
void func_800C878C(short turns) {
    if (!restricted()) {
        Iter800C878C iter;
        Iter800C878C *cursor = &iter;
        cursor->cursor = 0;
        while (func_800A8FC8(cursor, 0x7C)) {
            Obj800C878C *obj = next_object(cursor);
            if (accepts_update(obj)) func_800C8D78(obj, turns);
        }
    }
}

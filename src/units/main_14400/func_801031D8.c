#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 value;
} Dir;
typedef struct {
    s32 x;
    s32 y;
} Pair;
s32 func_800A4754(void *obj, void *pos, void *dir);
void *func_800A2594(void *out, void *from, Dir dir);
u8 *func_800B4928(Pair *);
s32 func_800A4314(u8 *, Pair *);
s32 func_800E20CC(void *obj);
s32 func_800B56F0(Pair *);
u8 *func_800B4D80(Pair *);
s32 func_800AD714(u8 *, Pair *);
s32 func_801031D8(u8 *self, Pair *pos, Dir *dir) {
    Pair front;
    u8 *obj;
    s32 result;

    if (!func_800A4754(self, pos, dir)) {
        return 0;
    }
    func_800A2594(&front, pos, *dir);
    obj = func_800B4928(pos);
    if (obj != 0) {
        if (obj[0x1E] & 0x7C) {
            return 0;
        }
        result = 0;
        if (func_800A4314(obj, &front)) {
            if (func_800E20CC(self) || !func_800B56F0(&front)) {
                result = 1;
            }
        }
        return result;
    }
    obj = func_800B4D80(pos);
    if (obj == 0) {
        return 0;
    }
    if (obj[0] != 0xF) {
        return 0;
    }
    return func_800AD714(obj, &front);
}

#include "common.h"

/* Floor-object placement pass (C++ TU: its first retry loop stays top-tested, as g++ emits it). */

/* Same layout as the canonical C views of the selection record at D_80142F18. */
struct SelectionRecord {
    unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode;
    signed char coordinates[2], status;
};

struct Pair {
    s32 x;
    s32 y;
    Pair() {}
};

/* g++ 2.x vtable entry; slot 1 is the virtual destructor: void (self, flags). */
struct VtblEntry {
    short delta;
    short index;
    void (*fn)(void *self, s32 flags);
};

struct Object {
    char fields_0[0x24];
    VtblEntry *vtbl;
};

extern "C" {
extern SelectionRecord D_80142F18;
void func_800AA4BC(void);
s32 func_800AA4D0(void);
Object *func_800AA4E4(void);
Object *func_800AA5F8(void);
s32 func_800A5B98(Object *object, Pair *position);
s32 func_800B1E80(Pair *position);
s32 func_800BAA98(void *object, void *position);
s32 func_800BAAE0(void *object, void *position);
void func_800A58FC(Object *object, Pair *position);
void func_800EE718(Object *object, s32 mode);
}

extern "C" void func_800BFA54(void *context)
{
    if ((D_80142F18.flags >> 2) & 1) {
        return;
    }
    func_800AA4BC();
    while (func_800AA4D0()) {
        Object *object = func_800AA4E4();
        if (object) {
            s32 attempts = 100;
            while (attempts-- != 0) {
                Pair position;
                if (func_800A5B98(object, &position)) {
                    s32 valid = 0;
                    if (!func_800B1E80(&position)) {
                        if (!func_800BAA98(context, &position)) {
                            valid = func_800BAAE0(context, &position) == 0;
                        }
                    }
                    if (valid) {
                        func_800A58FC(object, &position);
                        func_800EE718(object, 2);
                        break;
                    }
                } else {
                    /* delete object (virtual destructor, deleting flag 3) */
                    if (object) {
                        object->vtbl[1].fn((char *)object + object->vtbl[1].delta, 3);
                    }
                    break;
                }
            }
        }
    }
    Object *last = func_800AA5F8();
    if (last) {
        Pair position;
        s32 valid;
        s32 attempts = 100;
        while (attempts-- != 0) {
            if (func_800A5B98(last, &position)) {
                valid = 0;
                if (!func_800BAA98(context, &position)) {
                    valid = func_800BAAE0(context, &position) == 0;
                }
                if (valid) {
                    func_800A58FC(last, &position);
                    break;
                }
            } else {
                if (last) {
                    last->vtbl[1].fn((char *)last + last->vtbl[1].delta, 3);
                }
                break;
            }
        }
    }
}

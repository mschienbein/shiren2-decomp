#include "common.h"
#include "map_types.h"
#include "selection_records.h"

/* Floor-object placement pass (C++ TU: its first retry loop stays top-tested, as g++ emits it). */

/* g++ 2.x vtable entry; slot 1 is the virtual destructor: void (self, flags). */
struct VtblEntry_800BFA54 {
    short delta;
    short index;
    void (*fn)(void *self, s32 flags);
};

/* Object view: only the vtable pointer at +0x24 is read. */
struct ObjectView_800BFA54 {
    char fields_0[0x24];
    VtblEntry_800BFA54 *vtbl;
};

extern "C" {
extern SelectionRecord D_80142F18;
void func_800AA4BC(void);
s32 func_800AA4D0(void);
ObjectView_800BFA54 *func_800AA4E4(void);
ObjectView_800BFA54 *func_800AA5F8(void);
s32 func_800A5B98(ObjectView_800BFA54 *object, MapPos *position);
s32 func_800B1E80(MapPos *position);
s32 func_800BAA98(void *object, void *position);
s32 func_800BAAE0(void *object, void *position);
void func_800A58FC(ObjectView_800BFA54 *object, MapPos *position);
void func_800EE718(ObjectView_800BFA54 *object, s32 mode);
}

extern "C" void func_800BFA54(void *context)
{
    if ((D_80142F18.flags >> 2) & 1) {
        return;
    }
    func_800AA4BC();
    while (func_800AA4D0()) {
        ObjectView_800BFA54 *object = func_800AA4E4();
        if (object) {
            s32 attempts = 100;
            while (attempts-- != 0) {
                MapPos position;
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
    ObjectView_800BFA54 *last = func_800AA5F8();
    if (last) {
        MapPos position;
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

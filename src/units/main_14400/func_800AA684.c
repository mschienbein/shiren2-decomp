#include "common.h"

typedef struct Obj Obj;
typedef struct {
    unsigned char pad_00[8];
    short delta_08;
    short index_0A;
    void (*destroy_0C)(void *obj, s32 flags);
} VTable;
struct Obj {
    unsigned char pad_00[0x1C];
    unsigned short field_1C;
    unsigned char pad_1E[6];
    const VTable *field_24;
};
extern Obj *func_800AA63C(void);

Obj *func_800AA684(void) {
    s32 remaining = 32;
    Obj *obj;
    for (;;) {
        if (--remaining == -1) {
            return 0;
        }
        obj = func_800AA63C();
        if (obj == 0) {
            return 0;
        }
        if (obj->field_1C & 8) {
            obj->field_24->destroy_0C((char *)obj + obj->field_24->delta_08, 3);
        } else {
            return obj;
        }
    }
}

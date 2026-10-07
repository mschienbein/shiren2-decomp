#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u16 field_0;
    u16 field_2;
} Entry800EFDFC;

Entry800EFDFC *func_80044F5C(u8 arg0, u8 arg1);

u16 func_800EFDFC(u8 arg0, u8 arg1) {
    Entry800EFDFC *entry = func_80044F5C(arg0, arg1);

    if (entry != 0) {
        return entry->field_2;
    }
    return 0;
}

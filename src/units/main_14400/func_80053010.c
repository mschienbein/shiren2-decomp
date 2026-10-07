#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    u8 pad0[0x8];
    u8 field_8;
} Entry_80053010;

extern Entry_80053010 *func_80053034(s16 id);

u8 func_80053010(s16 id) {
    return func_80053034(id)->field_8;
}

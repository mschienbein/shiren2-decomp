#include "common.h"

typedef unsigned char u8;

/* 7-byte script records addressed by a 7- or 15-bit index. */
typedef struct Record7 {
    u8 bytes[7];
} Record7;

typedef struct Script8012964C {
    u8 pad_00[0x18];
    Record7 *records_18;
} Script8012964C;

typedef struct Obj8012964C {
    u8 pad_00[0x74];
    Script8012964C *script_74;
} Obj8012964C;

u8 *func_80129240(Obj8012964C *obj, u8 *p);

/* Script opcode (table D_801487D0): run record #index through func_80129240. */
u8 *func_8012964C(Obj8012964C *obj, u8 *p) {
    s32 index = *p++;

    if (index & 0x80) {
        index &= 0x7F;
        index <<= 8;
        index |= *p++;
    }
    func_80129240(obj, obj->script_74->records_18[index].bytes);
    return p;
}

#include "common.h"

/* ODD_C: the four header bytes (active flag, state, count, spare; func_8008C75C and the caller
   func_8008D2A8 read them with lbu) are tested here as one word: lw +0x00, mask 0xFFFF0000. */
typedef struct {
    union { u32 word; unsigned char bytes[4]; } field_00;
    unsigned char field_04;
} Object;
extern s32 func_8008C400(void);
extern void func_8008C75C(Object *);

s32 func_8008C89C(Object *object) {
    s32 result = 0;
    if ((object->field_00.word & 0xFFFF0000) == 0x01000000) {
        if (object->field_04 == 0 || func_8008C400()) {
            func_8008C75C(object);
            result = 1;
        }
    }
    return result;
}

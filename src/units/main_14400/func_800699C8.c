#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u16 threshold; u16 pad[3]; } Entry800699C8;
extern Entry800699C8 D_8013C498[];

u32 func_800699C8(u32 value) {
    u32 i;

    for (i = 1; i < 10; i++) {
        if (value < D_8013C498[i].threshold) {
            break;
        }
    }
    return i - 1;
}

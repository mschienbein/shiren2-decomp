#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

s16 *func_80045B48(void);
void func_80045AC8(s16 id);
void func_80045AE8(void) {
    s16 *ids = func_80045B48();
    while (*ids != -1) {
        func_80045AC8(*ids);
        ids++;
    }
}

#include "common.h"

typedef struct { char pad[0xC]; u32 *ptrC; s32 remain10; } Stream;
void func_800839C4(Stream *s);
u32 func_80083AB4(void *stream) {
    u32 v;
    if (((Stream *)stream)->remain10 <= 0) func_800839C4(stream);
    v = *((Stream *)stream)->ptrC++;
    ((Stream *)stream)->remain10 -= 4;
    return v;
}

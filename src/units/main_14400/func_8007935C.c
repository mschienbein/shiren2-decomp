#include "common.h"

typedef struct { unsigned char pad[0xB0]; } Slot;
extern Slot D_801DD378[];
void func_80074778(Slot *s);
void func_8007935C(s32 i) {
    func_80074778(&D_801DD378[i]);
}

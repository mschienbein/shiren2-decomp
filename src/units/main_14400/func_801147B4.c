#include "common.h"
/* Partial view of the D_8015D938-class object (text id byte at +0x29). */
typedef struct { unsigned char pad00[0x29]; unsigned char id_29; } Obj80114794;
/* func_80114794 returns the text pointer from func_800B0A70, not an integer. */
extern char *func_80114794(Obj80114794 *self);
extern char *func_80083F34(void *src, s32 length, char *dst);
extern char D_801CA630[0x10];
void *func_801147B4(Obj80114794 *self) {
    char *text = func_80114794(self);
    char *result;
    if (text != 0) {
        *func_80083F34(text, 4, D_801CA630) = 0;
        result = D_801CA630;
    } else {
        result = 0;
    }
    return result;
}

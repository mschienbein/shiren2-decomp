#include "common.h"

typedef unsigned char u8;

/* Partial view of the D_8015D938-class object: text id byte at +0x29. */
typedef struct {
    u8 pad00[0x29];
    u8 id_29;
} Obj80114794;

char *func_800B0A70(u8 id, s32 force);

char *func_80114794(Obj80114794 *obj) {
    return func_800B0A70(obj->id_29, 0);
}

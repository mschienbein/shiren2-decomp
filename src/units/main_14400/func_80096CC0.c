#include "common.h"

/* Text window subobject (func_800486A4 view): handle at +0xC. */
typedef struct {
    unsigned char fields_00[0xC];
    s32 handle_0C;
} Window80096CC0;

typedef struct {
    char pad00[0x54];
    Window80096CC0 window54;
} Obj80096CC0;

extern void func_80048728(void *);

/* Virtual slot: forwards to the embedded text window at +0x54. */
void func_80096CC0(Obj80096CC0 *obj) {
    func_80048728(&obj->window54);
}

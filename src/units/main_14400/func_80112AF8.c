#include "common.h"
#include "item_records.h"
extern char *func_80112A6C(unsigned char kind, s32 level);
extern char *func_80083C90(char *dst, char *src);
char *func_80112AF8(Item *self, char *out) {
    /* D_8015D6A0 and its subclass tables belong to the kind-0x11 level family. */
    func_80083C90(out, func_80112A6C(self->field01, self->payload.level.level0C));
    return out;
}

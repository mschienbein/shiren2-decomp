#include "common.h"
typedef struct {
    unsigned char field_00;
    unsigned char kind_01;
    unsigned char pad_02[0xA];
    s32 level_0C;
} Obj80112AF8;
extern char *func_80112A6C(unsigned char kind, s32 level);
extern char *func_80083C90(char *dst, char *src);
char *func_80112AF8(Obj80112AF8 *self, char *out) {
    func_80083C90(out, func_80112A6C(self->kind_01, self->level_0C));
    return out;
}

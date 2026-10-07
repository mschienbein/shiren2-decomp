#include "common.h"

typedef unsigned short u16;

extern s32 func_80049CB4(s32 command, ...);
extern void func_800498E4(s32 id, ...);
extern char *func_800AE674(void *obj);
extern char *func_80114BCC(void *obj, char *buffer, s32 flags);
extern s32 func_8011459C(void *obj, void *pos);

void func_801146A4(void *obj, void *pos, u16 id) {
    func_80049CB4(0xFE, pos);
    if (id != 0) {
        func_80049CB4(6);
        func_800498E4(id, func_80114BCC(obj, func_800AE674(obj), 0));
        func_80049CB4(7);
    }
    func_8011459C(obj, pos);
}

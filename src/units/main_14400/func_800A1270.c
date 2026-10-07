#include "common.h"

void func_800A12A4(void *req, void *from, void *to, unsigned short id, s32 (*filter)(void *));
void *func_800A1270(void *req, void *from, void *to, unsigned short id, s32 (*filter)(void *)) {
    func_800A12A4(req, from, to, id, filter);
    return req;
}

#include "common.h"
typedef struct { unsigned char kind; } Item800ACB8C;
extern s32 func_80114730(void *self, void *payload);
s32 func_800ACB8C(Item800ACB8C *self, void *payload) {
    if (self->kind == 9) {
        return func_80114730(self, payload);
    }
    return 0;
}

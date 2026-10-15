#include "common.h"

typedef unsigned char u8;

typedef struct {
    unsigned char unknown_00[0x40];
    unsigned short field_40;
} Func800E110CView;

extern u32 func_800E110C(const Func800E110CView *arg0);

/* Byte-sized accessor for bits 4..7 of the complete field_40. */
u8 func_800E2848(const Func800E110CView *obj) {
    return func_800E110C(obj);
}

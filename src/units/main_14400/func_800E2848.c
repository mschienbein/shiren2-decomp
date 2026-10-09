#include "common.h"

typedef unsigned char u8;

typedef struct {
    unsigned char unknown_00[0x41];
    unsigned char byte_41;
} Func800E110CView;

extern u32 func_800E110C(const Func800E110CView *arg0);

/* Byte-sized accessor for the high nibble of byte 0x41. */
u8 func_800E2848(const Func800E110CView *obj) {
    return func_800E110C(obj);
}

#include "common.h"
/* Two-byte record prefix: kind and variant bytes. */
typedef struct { unsigned char field00; unsigned char field01; } Pair;
extern char *func_800A9890(unsigned char kind, unsigned char variant);
char *func_800CC728(const Pair *pair) {
    return func_800A9890(pair->field00, pair->field01);
}

#include "common.h"

typedef unsigned char u8;
extern void func_80052644(u8 value);

/* Void facade: forwards the unsigned byte; no result is produced. */
void func_80045C68(u8 value) {
    func_80052644(value);
}

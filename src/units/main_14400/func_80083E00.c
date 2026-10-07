#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

u16 func_80083E00(u8 code) {
    u16 result;

    if (code == 0) {
        return 0;
    }
    if (code < 0x56) {
        return code + 0x8B;
    }
    if (code < 0xAC) {
        result = code - 0xF48;
        if ((result & 0xFF) >= 0x25) {
            result = code - 0xF47;
        }
        return result;
    }
    if (code < 0xB4) {
        return 0xF0B8;
    }
    if (code < 0xBE) {
        return code - 0xFE5;
    }
    if (code < 0xCA) {
        return code - 0x3E;
    }
    if (code < 0xD2) {
        return code - 0x103C;
    }
    if (code == 0xD2) {
        return code - 0x103B;
    }
    if (code < 0xE6) {
        return code - 0x1036;
    }
    return code - 0x100D;
}

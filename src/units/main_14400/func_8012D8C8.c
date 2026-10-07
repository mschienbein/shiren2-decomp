#include "common.h"

typedef unsigned char u8;
void func_8012D8C8(u8 *dst, u8 *src, u32 n){
    if (src < dst) {
        dst += n; src += n;
        while (n--) *--dst = *--src;
    } else {
        while (n--) *dst++ = *src++;
    }
}

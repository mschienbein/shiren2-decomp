#include "common.h"

typedef signed char s8;
typedef short s16;
typedef struct { s8 x; s8 y; } BytePair;
extern BytePair D_8014BF98;
void func_80051E9C(s16 id, BytePair pair);
void func_80052260(short arg0) {
    BytePair pair = D_8014BF98;
    func_80051E9C(arg0, pair);
}

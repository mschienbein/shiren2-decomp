#include "common.h"
extern s32 func_8008DF04(void *stream);
extern s32 func_8008E1E8(void *stream);
extern s32 func_8008E3B0(void *stream, void *out);
s32 func_8008E354(void *stream, void *out) {
    if (func_8008DF04(stream) != 0x45464154) {
        return -1;
    }
    if (func_8008E1E8(stream)) {
        return -1;
    }
    return func_8008E3B0(stream, out);
}

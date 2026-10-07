#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 cells[0x4C]; } Row800B4E18;
extern Row800B4E18 D_80145460[];
typedef struct { s32 row; s32 col; } Pos800B4E18;
void *func_800B4D80(void *pos);
void *func_800B4E18(Pos800B4E18 *pos) {
    void *result = func_800B4D80(pos);
    if (result != 0) {
        D_80145460[pos->row].cells[pos->col] = 0xFF;
    }
    return result;
}

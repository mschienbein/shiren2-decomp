#include "common.h"

/* Six 0xC-byte records; the old D_8014266C label is record zero's +8 field. */
typedef struct { unsigned short value; unsigned char pad02[6]; s32 index; } Label;
extern s32 D_8014264C[4];
extern s32 D_8014265C[2];
extern Label D_80142664[6];
extern unsigned short D_80154258[];
extern s32 func_800CC8A4(unsigned char);
extern void func_80097240(void *, Label *, s32 *, s32 *);

void func_8009EDE0(void *object) {
    s32 count = 0;
    s32 index = 0;
    do {
        if (func_800CC8A4(index & 0xFF)) {
            unsigned short value = D_80154258[index];
            D_80142664[count].index = index;
            D_80142664[count].value = value;
            count++;
        }
        index++;
    } while (index < 6);
    D_8014265C[0] = count;
    D_8014264C[0] = count;
    func_80097240(object, D_80142664, D_8014264C, D_8014265C);
}

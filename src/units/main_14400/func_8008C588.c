#include "common.h"
typedef unsigned char u8;
typedef struct { float x, y, z; } Vec3;
typedef struct { u8 pad00[0x10]; float matrix10[16]; u8 pad50[0x28]; } Record;
/* D_8013FEE4 bank: 0x100-byte header followed by eight 0x78-byte slots (see func_8008CF78). */
typedef struct { u8 header[0x100]; Record records[8]; } Manager;
extern s32 D_8013FEE0;
extern Manager *D_8013FEE4;
extern void func_8002FA20(float *, float, float, float, float, float, float, float);
void func_8008C588(s32 handle, Vec3 *position, float scale)
{
    float matrix[16];
    Record *record;
    if (D_8013FEE0 != 0) {
        record = &D_8013FEE4->records[handle];
        func_8002FA20(matrix, 0.0f, 0.0f, 0.0f, scale, position->x, position->y, position->z);
        record->matrix10[0] = matrix[0];
        record->matrix10[1] = matrix[1];
        record->matrix10[2] = matrix[2];
        record->matrix10[3] = matrix[3];
        record->matrix10[4] = matrix[4];
        record->matrix10[5] = matrix[5];
        record->matrix10[6] = matrix[6];
        record->matrix10[7] = matrix[7];
        record->matrix10[8] = matrix[8];
        record->matrix10[9] = matrix[9];
        record->matrix10[10] = matrix[10];
        record->matrix10[11] = matrix[11];
        record->matrix10[12] = matrix[12];
        record->matrix10[13] = matrix[13];
        record->matrix10[14] = matrix[14];
        record->matrix10[15] = matrix[15];
    }
}

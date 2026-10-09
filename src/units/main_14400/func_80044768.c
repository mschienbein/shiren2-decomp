#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef unsigned char u8;

typedef struct {
    s32 id;
    s32 x;
    s32 y;
} End;

typedef struct {
    End end;
    s32 foundC;
} Query;


extern s32 D_80138BAC;
End D_80138B40 = { 0, 0, 0 };

void func_800447EC(Query *obj);
End *func_80044840(Query *obj);
End *func_80044940(Query *obj);

End *func_80044768(Query *obj) {
    if ((D_80142F18.flags >> 2) & 1) {
        if (D_80138BAC != 0) {
            func_800447EC(obj);
            D_80138BAC = 0;
        }
        if (obj->foundC == 0) {
            return &D_80138B40;
        }
        return func_80044840(obj);
    }
    return func_80044940(obj);
}

#include "common.h"
typedef struct { s32 values[3]; } Table;
extern const Table D_8014ABAC;
extern void func_80083FEC(s32 value);
void func_800499C0(s32 index) {
    Table table = D_8014ABAC;
    func_80083FEC(table.values[index]);
}

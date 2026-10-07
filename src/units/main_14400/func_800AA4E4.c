#include "common.h"
typedef struct { unsigned char field_00[0x9C]; unsigned short field_9C; } Object;
/* D_80153664 contains six 8-byte {id, flag-address} records, including the terminator. */
typedef struct { s32 id; unsigned char *flag; } Entry800AA4E4;
extern Entry800AA4E4 *D_80142B18;
extern unsigned char D_80142F28;
extern char D_80147620[];
extern unsigned short func_801F2888(unsigned char);
extern s32 func_800A9070(s32 *, s32);
extern s32 func_800C587C(void *, unsigned char);
extern unsigned char func_800ABD94(s32);
extern Object *func_800A85A0(unsigned char, unsigned char);
Object *func_800AA4E4(void) {
    s32 value = D_80142B18->id;
    unsigned char other = *D_80142B18->flag;
    unsigned short id;
    Object *object;

    D_80142B18++;
    id = func_801F2888(value);
    if (id) {
        s32 local;
        if (D_80142F28 & (value - 0x18)) return 0;
        local = 0;
        if (func_800A9070(&local, value)) return 0;
        if (!func_800C587C(D_80147620, other)) return 0;
        object = func_800A85A0(value, func_800ABD94(value));
        if (!object) return 0;
        object->field_9C = id;
        return object;
    }
    return 0;
}

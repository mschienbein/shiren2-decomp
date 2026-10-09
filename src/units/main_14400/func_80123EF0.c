#include "common.h"
typedef unsigned char u8;
/* Entity table (+0x24) slot +0x94: decided contract s32 (void *self, s32, s32, u8, s32). */
typedef struct { unsigned char field_0[0x90]; short field_90; s32 (*field_94)(void *self, s32 a, s32 b, u8 c, s32 d); } VTable;
typedef struct { unsigned char field_0[0xA]; unsigned char field_A; unsigned char field_B[0x13]; unsigned char field_1E; unsigned char field_1F[5]; VTable *field_24; } Object;
extern s32 func_80049CB4(s32, ...);
extern s32 func_800EB944(Object *), func_8010A0B0(Object *);
extern void func_800498E4(s32, ...);
static inline s32 is_special(Object *arg) { s32 result = 0; if (((arg->field_1E >> 2) & 1) || arg->field_A == 0x1A) result = 1; return result; }
/* ODD_C: boolean helper; GCC 2.8.1 expands its result as sne (sltu rd,$zero,rs), as the original does. */
static inline u8 nonzero(u32 value) { return value != 0; }
static inline s32 is_ordinary(Object *arg)
{
    return !(arg->field_1E & 0xC) && nonzero(arg->field_1E & 0x7C);
}
/* D_8015FD88+0x44 (0x8015FDCC): func_80115EB0 supplies seven pointers and consumes
 * the s32 result. self, actor, source_position, direction and item are
 * caller-supplied but unused here. */
s32 func_80123EF0(void *self, void *actor, void *source_position, void *source,
                  void *direction, Object *arg, void *item)
{
    if (arg) {
        func_80049CB4(0xF2, source);
        if (is_special(arg)) {
            s32 result;
            s32 message;
            func_80049CB4(0x6F, arg);
            if ((arg->field_1E >> 2) & 1) result = func_800EB944(arg);
            else result = func_8010A0B0(arg);
            message = 0x224;
            if (result) message = 0xF0;
            func_800498E4(message);
        } else if (is_ordinary(arg)) {
            arg->field_24->field_94((unsigned char *)arg + arg->field_24->field_90, 0, 2, 0xFE, 0);
            return 1;
        } else func_800498E4(0x224);
    }
    return 1;
}

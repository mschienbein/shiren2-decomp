#include "func_801ED008_13EF28/types.h"

/* Script command dispatcher of the message/event interpreter (overlay B). The
 * command byte D_801F5CCD_147BED selects the case; arguments are the s32 words at
 * D_801F51E8_147108+0x108.. (several read through their low byte). Returns 4 to
 * continue, 6 to wait, 0 to stop. */

extern char D_801F51F0_147110[];
extern void *D_801476B8;
/* Main selection record (canonical view: func_800DF9F0.c and others). */
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05; unsigned short field_06; unsigned char mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;

typedef struct { s8 x; s8 y; } BytePair;

char *func_800A3B20(void *u);
void func_80045A44(void);
void func_800458BC(s32 arg);
void func_80045904(s32 a);
void func_80045948(s32 id, BytePair pair);
void func_80048380(void);
void func_800838A8(u8 value);
char *func_80048480(u16 id);
s32 func_800327C0(char *dst, const char *fmt, ...);
u32 func_80032D70(const char *s);

void func_801E511C_13703C(s32 arg0, s32 arg1);
void func_801E60B0_137FD0(BytePair *position, u8 x, u8 y);
void func_801E7BF8_139B18(void);
void func_801E82E0_13A200(s32 arg);
void func_801E8ED4_13ADF4(s32 arg);
void func_801E8F20_13AE40(s32 arg);
s32 func_801EA024_13BF44(void);
void func_801EB348_13D268(const u8 *text);
s32 func_801EB444_13D364(s32 arg);
void func_801EB6A0_13D5C0(u8 arg);
void func_801EC510_13E430(void);
void func_801EC54C_13E46C(void);
s32 func_801ECF5C_13EE7C(const char *text, s32 index);
void func_801EE8CC_1407EC(s32 arg);
void func_801EF7F0_141710(s32 arg);

/* Recount the message text after a colour code was copied in. */
static inline void text_measure(D_801F51E8_s *p)
{
    p->len_11E = func_80032D70(p->buf_126);
}

s32 func_801ED008_13EF28(void)
{
    char *name;
    s32 i;

    switch (D_801F5CCD_147BED) {
    case 0x10:
        D_801F51E8_147108->b997 = D_801F51E8_147108->arg_108;
        if (D_801F51E8_147108->arg_110 != 0) {
            func_801E82E0_13A200(D_801F51E8_147108->arg_110 - 1);
        }
        break;
    case 0x11:
        D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E++] = '\n';
        if (D_801F51E8_147108->b99A) {
            __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5F28_147E48, sizeof(D_801F5F28_147E48));
            text_measure(D_801F51E8_147108);
        }
        if (D_801F51E8_147108->codes_2A6[D_801F51E8_147108->b9F4 - 1] == ' ') {
            __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5ED0_147DF0, sizeof(D_801F5ED0_147DF0));
            text_measure(D_801F51E8_147108);
        }
        break;
    case 0x12:
        if (D_801F51E8_147108->b9F3) {
            func_801E511C_13703C(0, 8);
        } else {
            func_801EA024_13BF44();
        }
        break;
    case 0x13:
        D_801F51E8_147108->len_11E = 0;
        D_801F51E8_147108->b99D = 1;
        if (D_801F51E8_147108->b99A) {
            __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5F28_147E48, sizeof(D_801F5F28_147E48));
            text_measure(D_801F51E8_147108);
        }
        break;
    case 0x14:
        D_801F51E8_147108->arg_108 = 1;
    case 0x15:
        if (D_801F51E8_147108->b9F3) {
            func_801E511C_13703C(D_801F51E8_147108->arg_108, 4);
        } else {
            func_801E8F20_13AE40(D_801F51E8_147108->arg_108);
        }
        break;
    case 0x16:
        name = func_800A3B20(D_801476B8);
        if (D_801F51E8_147108->arg_108 == 0) {
            for (i = 0; i < 4; i++) {
                func_801ECF5C_13EE7C(name, i);
            }
        } else {
            func_801ECF5C_13EE7C(name, D_801F51E8_147108->arg_108 - 1);
        }
        func_801EE8CC_1407EC(1);
        break;
    case 0x17:
        D_801F51E8_147108->arg_108 = 1;
    case 0x18:
        func_801EC510_13E430();
        if (D_801F51E8_147108->b9F3) {
            D_801F51E8_147108->arg_108 = D_801F51E8_147108->b9F3;
        }
        if (func_801EB444_13D364(D_801F51E8_147108->arg_108)) {
            D_801F51EC_14710C->count_4--;
            return 6;
        }
        break;
    case 0x19:
        func_801EB6A0_13D5C0(0);
        if (D_801F51E8_147108->arg_108 == -1) {
            func_80045A44();
        } else {
            func_800458BC(D_801F51E8_147108->arg_108);
            D_80142F18.status = D_801F51E8_147108->arg_108;
            func_80045904(D_801F51E8_147108->arg_108);
        }
        break;
    case 0x1A:
        {
            BytePair pos;
            BytePair pair;
            func_801E60B0_137FD0(&pos, D_801F51E8_147108->arg_114, D_801F51E8_147108->arg_110);
            /* ODD_C: ROM copies the filled pair (lb/sb, sp+0x10 -> sp+0x18) before
             * packing it into a1. Passing `pos` itself (1416 B) or a by-value return
             * passed directly (1420 B) omits that copy; only a second local
             * reproduces it. */
            pair = pos;
            func_80045948(D_801F51E8_147108->arg_108, pair);
        }
        break;
    case 0x1B:
        if (D_801F51E8_147108->b9F3) {
            func_801E511C_13703C(0, 8);
        } else {
            func_801EA024_13BF44();
        }
        D_801F51E8_147108->b99D = 1;
        if (D_801F51E8_147108->b99A) {
            __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5F28_147E48, sizeof(D_801F5F28_147E48));
            text_measure(D_801F51E8_147108);
        }
        break;
    case 0x1C:
        D_801F51E8_147108->b99A = D_801F51E8_147108->arg_10C;
        if (D_801F51E8_147108->b9F3) {
            func_801E511C_13703C(D_801F51E8_147108->arg_108, 0x10);
            func_801E7BF8_139B18();
            break;
        }
        if (D_801F51E8_147108->b9F1) {
            if (D_801F51E8_147108->b990 == 0) {
                func_801EC54C_13E46C();
            } else {
                do {
                    func_801E8ED4_13ADF4(1);
                } while (D_801F51E8_147108->b9F1);
            }
        }
        D_801F51E8_147108->b990 = D_801F51E8_147108->arg_108;
        func_80048380();
        D_801F51E8_147108->len_11E = 0;
        switch (D_801F51E8_147108->b990) {
        case 2:
            D_801F51E8_147108->b997 = 0;
            D_801F51E8_147108->b999 = 0;
            func_800838A8(1);
            break;
        case 4:
            func_800838A8(0);
            D_801F51E8_147108->b997 = 0;
            D_801F51E8_147108->b999 = 0;
            break;
        case 3:
            D_801F51E8_147108->b997 = 0;
            D_801F51E8_147108->b999 = 0;
            func_800838A8(1);
            break;
        default:
            D_801F51E8_147108->b997 = 1;
            D_801F51E8_147108->b999 = 1;
            func_800838A8(1);
            func_801E7BF8_139B18();
            break;
        }
        if (D_801F51E8_147108->b99A) {
            __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5F28_147E48, sizeof(D_801F5F28_147E48));
            text_measure(D_801F51E8_147108);
        }
        break;
    case 0x1D:
        func_801EF7F0_141710(D_801F51E8_147108->arg_108);
        break;
    case 0x1E:
        return 0;
    case 0x1F: {
        char text[100];
        func_800327C0(text, func_80048480(0x4F9), D_801F51F0_147110);
        func_801EB348_13D268((const u8 *)text);
        break;
    }
    }
    return 4;
}

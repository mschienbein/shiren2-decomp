#include "func_801ED720_13F640/types.h"

/* Second command dispatcher (commands 0x20..0x27: text colour, message end,
 * choice entries, unit position, ...); same interpreter state as func_801ED008_13EF28. */


typedef struct { s32 x, y; } Pos;
/* Unit returned by func_800A8CB0; only its map position is read here. */
typedef struct { Pos pos; } Unit;

void func_800265E0(void *, s32);
void func_80048380(void);
s32 func_80049CB4(s32 id, ...);
u32 func_80032D70(const char *s);
void *func_800A8CB0(s32 cell);
s32 func_800A8974(void *obj);

void func_801E5564_137484(D_801F51E8_s *p);
void func_801E5760_137680(D_801F51E8_s *p);
void func_801E82E0_13A200(s32 arg);
s32 func_801E9D34_13BC54(void);
void func_801EB6A0_13D5C0(u8 arg);
s32 func_801EBC44_13DB64(void);
u16 func_801ED5A0_13F4C0(void);

/* Recount the message text after a colour code was copied in. */
static inline void text_measure(D_801F51E8_s *p)
{
    p->len_11E = func_80032D70(p->buf_126);
}

s32 func_801ED720_13F640(void)
{
    /* ROM 0x13F650 / VRAM 0x801ED730 writes s0 = 0 in the first
     * jal delay slot, so this initialization executes even on early return.
     * Commands 0x23/0x24 do not consume it; readiness-using paths replace
     * it at 0x801ED764. Preserve the executed ROM initialization without
     * claiming that its zero is a live readiness fallback. */
    u8 ready = 0;
    u8 slot;
    Unit *unit;
    Pos pos;

    if (func_801ED5A0_13F4C0() == 1) {
        return 4;
    }
    if (D_801F5CCD_147BED != 0x23 && D_801F5CCD_147BED != 0x24) {
        ready = func_801EBC44_13DB64();
    }
    switch (D_801F5CCD_147BED) {
    case 0x20:
        if (ready == 1) {
            switch (D_801F51E8_147108->arg_108) {
            case 0:
                __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5ED0_147DF0, sizeof(D_801F5ED0_147DF0));
                break;
            case 1:
                __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5F98_147EB8, sizeof(D_801F5F98_147EB8));
                break;
            case 2:
                __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5F9C_147EBC, sizeof(D_801F5F9C_147EBC));
                break;
            case 3:
                __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5FA0_147EC0, sizeof(D_801F5FA0_147EC0));
                break;
            }
        } else {
            __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5FA4_147EC4, sizeof(D_801F5FA4_147EC4));
        }
        text_measure(D_801F51E8_147108);
        break;
    case 0x21:
    case 0x22:
        if (D_801F5CCD_147BED == 0x22) {
            D_801F51E8_147108->wA00 = 0;
        } else {
            D_801F51E8_147108->wA00 = 1;
        }
        if (ready == 1) {
            D_801F51E8_147108->b9FA = D_801F51E8_147108->arg_10C;
            D_801F51E8_147108->b99C = D_801F51E8_147108->arg_108;
            D_801F51E8_147108->b99E = 0;
            D_801F51E8_147108->b99F = 0;
            D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E++] = 0;
            D_801F51E8_147108->w9C8[D_801F51E8_147108->b99E] = D_801F51E8_147108->len_11E;
            func_800265E0(D_801F51E8_147108->w9A0, 10);
            return 4;
        }
        func_801E82E0_13A200(1);
        D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E++] = 0;
        slot = func_801E9D34_13BC54();
        if (slot != 0xFF) {
            D_801F51EC_14710C->count_4 += D_801F51E8_147108->w9A0[slot] - 1;
        }
        D_801F51E8_147108->buf_126[0] = 0;
        D_801F51E8_147108->len_11E = 0;
        D_801F51E8_147108->b99D = 1;
        func_80048380();
        if (!D_801F51E8_147108->b99A) {
            return 4;
        }
        __builtin_memcpy(&D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E], D_801F5F28_147E48, sizeof(D_801F5F28_147E48));
        text_measure(D_801F51E8_147108);
        break;
    case 0x23:
        D_801F51E8_147108->arg_114 = 1;
        D_801F51E8_147108->b125 = 0;
        D_801F51E8_147108->b99F = D_801F51E8_147108->b99E;
    case 0x24:
        func_801EB6A0_13D5C0(3);
        D_801F51E8_147108->w9A0[D_801F51E8_147108->b99E] = D_801F51E8_147108->arg_114;
        D_801F51E8_147108->buf_126[D_801F51E8_147108->len_11E++] = 0;
        D_801F51E8_147108->w9C8[D_801F51E8_147108->b99E++] = D_801F51E8_147108->len_11E;
        break;
    case 0x25:
        if (ready == 1) {
            D_801F51E8_147108->b002 = D_801F51E8_147108->arg_108;
            func_801E5760_137680(D_801F51E8_147108);
            if (D_801F51E8_147108->b001 != 0xFF) {
                unit = func_800A8CB0(D_801F51E8_147108->b001);
                if (!func_800A8974(unit)) {
                    pos.x = unit->pos.x;
                    pos.y = unit->pos.y;
                    D_801F51E8_147108->h010 = pos.y;
                    D_801F51E8_147108->h012 = pos.x;
                }
            }
            break;
        }
        func_801E5564_137484(D_801F51E8_147108);
        return 4;
    case 0x26:
        if (ready != 1) {
            func_80049CB4(2);
        }
        return 4;
    case 0x27:
        if (ready == 1) {
            D_801F51E8_147108->b9F3 = D_801F51E8_147108->arg_108;
        } else {
            D_801F51E8_147108->b9F3 = 0;
        }
        break;
    }
    return 4;
}

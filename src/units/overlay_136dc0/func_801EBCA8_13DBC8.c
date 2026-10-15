/* func_801EBCA8_13DBC8 (overlay_136dc0): event condition command. Dispatch is on D_801F5CCD_147BED - 2
 * through jtbl_801F5ED8_147DF8. When the condition fails, the code after the switch skips
 * operand 3 - 1 script positions.
 * __builtin_strcpy with the literal "#3c" emits the inline lwl/lwr copy (canonical precedent
 * for the builtin: __builtin_memcpy in func_800347D0). The literal is this function's
 * .rodata at 0x801F5ED0. __builtin_memcpy(.., "#3c", 4) also matches.
 * Data note: ROM explicitly supplies D_801F51EC_14710C at 801EBD58. The corrected private s08
 * func_801EB638_13D558 keeps that OvlScriptRecord * parameter, documented as unused by its body.
 *
 * ODD_C: cases 2 and 7 keep each test in a flag (`off`). As a stored value, do_store_flag
 * emits xori/sltu, and combine reduces the test to xori+bnez/beqz. Direct jump-form tests
 * emit li+bne/bnez: measured 406 words, text 1736. Explicit `(x ^ 1) != 0` tests also
 * match (0 words).
 * ODD_C: cases 5 and inner 8/9 write `if (a >= b) return 4; break;`. The `<`-then-break
 * form gives the same tests, but jump2 cross-jumps their tails into case 12's
 * call-result tail: measured 345 words, text 1732.
 * ODD_C: the u8 results of func_801E53A4_1372C4 are held in s32 locals; ROM zero-extends right
 * after each call. u8 locals measured 284 words, text 1756.
 * ODD_C: case 9 reads the state pointer into `st` before the strlen call, so ROM holds it
 * in s0 across the calls. A direct D_801F51E8_147108->field11E += measured 14 words. An
 * early-return then-arm instead of if/else measured 225 words, text 1744.
 * ODD_C: case 13 holds the state pointer across the func_801E9F70_13BE90 call (s0). Direct
 * global reads measured 87 words, text 1748.
 */
#include "func_801EBCA8_13DBC8/types.h"

extern s32 func_801E8D0C_13AC2C(u16 id, u16 selector);
extern Unit *func_800A8CB0(s32 cell);
extern s32 func_800A8974(void *obj);
extern OvlScriptRecord *func_801EB638_13D558(OvlScriptRecord *record);
extern s32 func_801EA1A4_13C0C4(u16 id);
extern void func_801EC818_13E738(void);
extern u8 func_801E53A4_1372C4(s32 position);
extern void func_801EB6A0_13D5C0(u8 index);
extern s32 func_801E8A28_13A948(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h);
extern s32 func_800D8144(s32 index);
extern void *func_800A83D0(s32 arg, u8 flags);
extern char *func_800A3CD0(Unit *unit);
extern char *func_80083C90(char *dst, char *src);
extern u32 func_80032D70(const char *s);
extern s32 func_801F5054_146F74(s32 a, s32 b, s32 c);
extern s32 func_801F48A4_1467C4(void);
extern s32 func_801EA364_13C284(s32 owner, s32 arg);
extern s32 func_801EA440_13C360(s32 owner, s32 arg);
extern s32 func_801EA728_13C648(s32 owner, s32 amount, s32 nested);
extern void func_801EC374_13E294(void);
extern u8 func_801E9F70_13BE90(s32 flag);
extern void func_801E7C6C_139B8C(s32 arg);
extern void func_801EC510_13E430(void);
extern s32 func_801E51D8_1370F8(void);
extern s32 func_801E51CC_1370EC(void);
extern s32 func_800D8088(u8 a, u8 b);

s32 func_801EBCA8_13DBC8(void) {
    Unit *unit;
    s32 a, b, c, d;
    SelectionRecord rec;

    switch (D_801F5CCD_147BED) {
    case 2:
        unit = func_800A8CB0((u8)func_801E8D0C_13AC2C((u16)D_801F51E8_147108->field108[0].word, 0));
        if (func_800A8974(unit) != 0) {
            return 4;
        }
        {
            s32 off = ((unit->flags1E >> 3) & 1) ^ 1;
            if (off) {
                return 4;
            }
            off = ((unit != 0) ? &unit->u78.p.c84 : 0)->x4 != 1;
            if (off) {
                return 4;
            }
        }
        break;
    case 3:
        D_801F51EC_14710C = func_801EB638_13D558(D_801F51EC_14710C);
        if (D_801F51EC_14710C == 0) {
            return 0;
        }
        D_801F51E8_147108->field9F4 = 0;
        return 4;
    case 4:
        if (func_801EA1A4_13C0C4(0) > D_801F51E8_147108->field108[0].word) {
            break;
        }
        return 4;
    case 5:
        if (func_801EA1A4_13C0C4(0) >= D_801F51E8_147108->field108[0].word) {
            return 4;
        }
        break;
    case 6:
        if (D_801F51E8_147108->field108[0].word == 10) {
            func_801EC818_13E738();
        }
        switch (D_801F51E8_147108->field108[2].word) {
        case 6:
            if (D_801F51E8_147108->field108[1].word == D_801F5228_147148[D_801F51E8_147108->field108[0].byte[3]]) {
                break;
            }
            return 4;
        case 7:
            if (D_801F51E8_147108->field108[1].word != D_801F5228_147148[D_801F51E8_147108->field108[0].byte[3]]) {
                break;
            }
            return 4;
        case 8:
            if (D_801F51E8_147108->field108[1].word >= D_801F5228_147148[D_801F51E8_147108->field108[0].byte[3]]) {
                return 4;
            }
            break;
        case 9:
            if (D_801F5228_147148[D_801F51E8_147108->field108[0].byte[3]] >= D_801F51E8_147108->field108[1].word) {
                return 4;
            }
            break;
        }
        break;
    case 8:
        a = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        b = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        c = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        d = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        func_801EB6A0_13D5C0(2);
        func_801EB6A0_13D5C0(3);
        func_801E8A28_13A948(D_801F51E8_147108->field108[0].word, D_801F51E8_147108->field108[1].word, D_801F51E8_147108->field108[2].word,
                      D_801F51E8_147108->field108[3].word, a, b, c, d);
        return 4;
    case 9: {
        s32 n;
        Unit *named;
        OvlState *st;
        n = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        func_801EB6A0_13D5C0(3);
        D_801F51E8_147108->field9A0[D_801F51E8_147108->field99E] = D_801F51E8_147108->field108[3].word;
        D_801F51E8_147108->field126[D_801F51E8_147108->field11E++] = 0;
        D_801F51E8_147108->field9C8[D_801F51E8_147108->field99E++] = D_801F51E8_147108->field11E;
        if (n == 1) {
            __builtin_strcpy((char *)&D_801F51E8_147108->field126[D_801F51E8_147108->field11E], "#3c");
            D_801F51E8_147108->field11E += 3;
        } else {
            named = func_800A83D0(0x29, func_800D8144(n - 2));
            if (func_800A8974(named) != 0) {
                return 4;
            }
            func_80083C90((char *)&D_801F51E8_147108->field126[D_801F51E8_147108->field11E], func_800A3CD0(named));
            st = D_801F51E8_147108;
            st->field11E += func_80032D70(func_800A3CD0(named));
            if (named != 0) {
                named->vtable->fn08((u8 *)named + named->vtable->delta08, 3);
            }
        }
        return 4;
    }
    case 7:
        {
            s32 off = func_801F5054_146F74(D_801F51E8_147108->field108[0].word, D_801F51E8_147108->field108[1].word, D_801F51E8_147108->field108[2].word) != 1;
            if (off) {
                return 4;
            }
        }
        break;
    case 10:
        if (func_801F48A4_1467C4() != 0) {
            break;
        }
        return 4;
    case 11:
        rec = D_80142F18;
        if (rec.mode != D_801F51E8_147108->field108[0].word) {
            return 4;
        }
        func_801EB6A0_13D5C0(1);
        func_801EB6A0_13D5C0(2);
        if (D_801F51E8_147108->field108[1].word != -1 && rec.coordinates[0] != D_801F51E8_147108->field108[1].word) {
            return 4;
        }
        if (D_801F51E8_147108->field108[2].word != -1 && rec.coordinates[1] != D_801F51E8_147108->field108[2].word) {
            return 4;
        }
        break;
    case 12: {
        s32 x;
        func_801EB6A0_13D5C0(1);
        x = D_801F51E8_147108->field108[1].word;
        if (x == -1) {
            if (func_801EA364_13C284(D_801F51E8_147108->field108[0].word, -1) != 0) {
                break;
            }
            return 4;
        } else if (x == -2) {
            if (func_801EA728_13C648(D_801F51E8_147108->field108[0].word, D_801F51E8_147108->field108[2].word, 0) == 0) {
                break;
            }
            return 4;
        } else if (x == 0) {
            if (func_801EA728_13C648(D_801F51E8_147108->field108[0].word, D_801F51E8_147108->field108[2].word, 1) == 0) {
                break;
            }
            return 4;
        } else if (x > 0xF8) {
            if (func_801EA440_13C360(D_801F51E8_147108->field108[0].word, x - 0xF8) != 0) {
                break;
            }
            return 4;
        } else {
            if (func_801EA364_13C284(D_801F51E8_147108->field108[0].word, x) != 0) {
                break;
            }
            return 4;
        }
    }
    case 13: {
        OvlState *st;
        if (D_801F51E8_147108->field108[0].word == 0x49) {
            func_801EC374_13E294();
        }
        st = D_801F51E8_147108;
        if (func_801E9F70_13BE90(st->field108[0].word) == st->field108[1].word) {
            break;
        }
        return 4;
    }
    case 14: {
        s32 x;
        func_801EB6A0_13D5C0(0);
        x = D_801F51E8_147108->field108[0].word;
        if (x == -1) {
            func_801E7C6C_139B8C(1);
            func_801EC510_13E430();
            if (func_801E51D8_1370F8() == x) {
                return 4;
            }
            func_801E7C6C_139B8C(0);
            break;
        }
        if (func_801E51CC_1370EC() == D_801F51E8_147108->field108[0].word) {
            break;
        }
        return 4;
    }
    case 15:
        if (func_800D8088(D_801F51E8_147108->field108[0].word, D_801F51E8_147108->field108[1].word) != 0) {
            return 4;
        }
        break;
    }
    func_801EB6A0_13D5C0(3);
    D_801F51EC_14710C->field4 += D_801F51E8_147108->field108[3].word - 1;
    D_801F51E8_147108->field9F4 = 0;
    return 4;
}

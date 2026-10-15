#include "func_801EB208_13D128/types.h"
extern u8 D_801F5228_147148[32];
extern char *func_80083F34(u8 *source, s32 length, char *destination);
extern s32 func_80083D54(u8 *a, u8 *b);

/* The unsigned-byte table access is shared with the table_get idiom in
 * func_801F258C_1444AC, func_801F2714_144634 and func_801F2888_1447A8. */
static inline u8 table_get(u8 index)
{
    return D_801F5228_147148[index];
}

void func_801EB208_13D128(void)
{
    /* The final multibyte code can write index 20 (ROM 0x13D1DC..0x13D1F8). */
    u8 encoded[21];
    u8 source[5];
    char expanded[20];
    s32 i;
    func_800265E0(expanded, sizeof(expanded));
    source[4] = 0;
    for (i = 0; ; i++) {
        if (i < 4) {
            source[i] = table_get(i + 4);
        } else {
            break;
        }
    }
    for (i = 0; i < 20; i++) {
        u8 code = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        encoded[i] = code;
        if ((code & 0xF0) == 0xF0) {
            encoded[++i] = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        } else if (code == 0x11) {
            encoded[i] = 0;
            break;
        }
    }
    func_80083F34(source, 20, expanded);
    if (func_80083D54((u8 *)expanded, encoded) == 0) {
        D_801F51EC_14710C = func_801EB5D4_13D4F4(D_801F51E8_147108->field108[1].half[1]);
    }
}

#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 field_00; u8 fields_01[14]; signed char field_0F; } Item;
typedef struct { u8 fields_00[0x74]; Item *field_74; } State;
typedef union { char text[0x200]; u16 codes[0x100]; } Text;

extern unsigned char D_80148450[];
extern const char D_80151FE4[], D_80151FEC[];
extern s32 func_800ACEB4(Item *item);
extern s32 func_8010BBF4(Item *item, unsigned char *list);
extern s32 func_8010BB60(Item *item, u8 *list);
extern char *func_8010C60C(Item *item, char *out, unsigned char limit);
extern s32 func_800A2910(u8 kind, u8 id, u16 *out);
extern char *func_80048480(u16 id);
extern char *func_800AE7BC(Item *item);
extern s32 func_800327C0(char *dst, const char *fmt, ...);

static inline s32 item_category(Item *item) { return (u32)(item->field_00 - 3) < 2; }
static inline Item *get_item(State *state) { return state->field_74; }

/* Widget text slot (+0x5C): writes the text of row `index` into out. */
void func_80096E28(State *state, s32 index, char *out)
{
    Text first, second;
    Item *current;
    s32 type, special;

    *out = 0;
    type = func_800ACEB4(state->field_74) ^ 2;
    current = get_item(state);
    special = item_category(current);
    {
        Item *item = current;

        if (!type && special && item->field_0F) {
            switch (index) {
            case 1:
                func_8010C60C(item, first.text, 0);
                func_800327C0(out, D_80151FE4, first.text);
                break;
            case 0: {
                s32 count = (u8)func_8010BBF4(item, D_80148450);

                if (count != (u8)func_8010BB60(item, D_80148450)) {
                    func_800A2910(item->field_00, D_80148450[0], first.codes);
                    func_800327C0(out, D_80151FE4, func_80048480(first.codes[1]));
                }
                break;
            }
            default: {
                unsigned char *codes = D_80148450;
                s32 count = (u8)func_8010BBF4(item, codes);
                s32 other = (u8)func_8010BB60(item, codes);
                s32 offset;

                if (count == other) offset = index - 2;
                else offset = index - 1;
                if (count >= offset) {
                    char *name;
                    unsigned char *code = codes + offset;

                    func_800A2910(item->field_00, *code, first.codes);
                    name = func_80048480(first.codes[0]);
                    func_800327C0(out, D_80151FEC, name, func_80048480(first.codes[1]));
                }
                break;
            }
            }
        } else if (special) {
            if (index == 0) {
                func_800327C0(first.text, D_80151FE4, func_800AE7BC(state->field_74));
                func_8010C60C(item, second.text, 0);
                func_800327C0(out, first.text, second.text);
            }
        } else if (index == 0) {
            func_800327C0(out, D_80151FE4, func_800AE7BC(state->field_74));
        }
    }
}

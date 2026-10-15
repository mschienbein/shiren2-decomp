#include "func_801F3664_145584/types.h"

/* Event opcodes 0x65/0x66 and 0x73..0x75 (dispatched by func_801F45F4_146514). */

extern "C" void func_801F3664_145584(void) {
    OvlBPos pos;
    s32 effect;
    s32 i;
    s32 end;

    switch (D_801F5CCD_147BED) {
    case 0x73:
        D_801F51E8_147108->unk993 = D_801F51E8_147108->args[0].word;
        func_801E69E8_138908(D_801F51E8_147108->unk993);
        break;
    case 0x74: {
        s32 model = SCRIPT_BYTE();
        model = (model << 8) + SCRIPT_BYTE();
        s32 count = SCRIPT_BYTE();
        f32 at[3];
        func_801EB6A0_13D5C0(0);
        func_801EB6A0_13D5C0(1);
        func_801EB6A0_13D5C0(2);
        at[0] = D_801F51E8_147108->args[0].word;
        at[1] = D_801F51E8_147108->args[1].word;
        at[2] = D_801F51E8_147108->args[2].word;
        func_801E6C44_138B64(model, (D_801F51E8_147108->args[3].word + 4) % 8, 0, count, at);
        break;
    }
    case 0x75:
        pos.y = D_801F51E8_147108->args[0].word;
        pos.x = D_801F51E8_147108->args[1].word;
        if (D_801F51E8_147108->args[2].word != 0) {
            func_800B5260(&pos, D_801F51E8_147108->args[2].word);
        } else {
            func_800B5650(&pos);
        }
        break;
    case 0x65: {
        s32 count;
        effect = SCRIPT_BYTE();
        count = SCRIPT_BYTE();
        if (func_801F2E80_144DA0(D_801F51E8_147108->args[0].word)) {
            Rec16 desc;
            OvlBUnitHead *unit;
            func_801F360C_14552C(D_801F51E8_147108->args, &desc);
            unit = func_801F1510_143430(&desc, count, 0);
            if (unit != 0) {
                u8 id = func_800A8C00(unit);
                unit->unk52 = 1;
                func_801E5CC0_137BE0(id, effect, 0xFF);
            }
        } else if (func_801F2E30_144D50(D_801F51E8_147108->args[0].word)) {
            OvlBUnitHead *unit;
            i = (u8)func_801E8D0C_13AC2C(D_801F51E8_147108->args[0].word, 0);
            unit = func_800A8CB0((u8)i);
            if (!func_800A8974(unit)) {
                if (D_801F51E8_147108->args[0].word == 0) {
                    func_801E6394_1382B4(0, 0);
                }
                pos.y = D_801F51E8_147108->args[1].word;
                pos.x = D_801F51E8_147108->args[2].word;
                unit->unk52 = 4;
                func_800A58FC(unit, &pos);
                func_801E7200_139120(i, pos.y, pos.x, pos.y, pos.x);
                OvlBDir dir(D_801F51E8_147108->args[3].word);
                func_800A665C(unit, &dir);
                unit->unk52 = 1;
                func_801E5CC0_137BE0(i, effect, 0xFF);
            }
        } else {
            Rec16 desc;
            OvlBUnitHead *unit;
            func_801F360C_14552C(D_801F51E8_147108->args, &desc);
            unit = func_801F1510_143430(&desc, 1, 0);
            if (unit != 0) {
                u8 id = func_800A8C00(unit);
                unit->unk52 = 1;
                func_801E5CC0_137BE0(id, effect, 0xFF);
                func_80049CB4(0xD7, unit);
            }
        }
        break;
    }
    case 0x66:
        i = 0;
        end = 1;
        effect = SCRIPT_BYTE();
        if (D_801F51E8_147108->args[0].word == 0xFF) {
            end = 30;
        } else if (D_801F51E8_147108->args[0].word != 0) {
            i = (u8)func_801E8D0C_13AC2C(D_801F51E8_147108->args[0].word, 0);
            if (i == 0) {
                break;
            }
            end = i + 1;
        }
        for (; i < end; i++) {
            OvlBUnitHead *unit = func_800A8CB0((u8)i);
            if (func_800A8974(unit)) {
                continue;
            }
            func_801F34D8_1453F8(unit, 0);
            func_801E6708_138628(i, 0, unit->facing.v, unit->unk0A);
            if (effect == 0) {
                func_801E7A28_139948(i);
                if (i == 0) {
                    func_801E6394_1382B4(0, 1);
                } else {
                    /* delete unit: virtual destructor (vtable slot 1) with the deleting flag 3. */
                    if (unit != 0) {
                        unit->vt[1].fn((u8 *)unit + unit->vt[1].delta, 3);
                    }
                    func_80049CB4(2);
                }
            } else {
                func_801E5CC0_137BE0(i, effect, 0);
            }
        }
        break;
    }
    func_80049CB4(2);
}

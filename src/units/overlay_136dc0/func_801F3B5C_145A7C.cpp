#include "func_801F3B5C_145A7C/types.h"

/* Unit event opcodes 0x60..0x7D for the unit named by args[0] (dispatched by func_801F45F4_146514). */

extern "C" void func_801F3B5C_145A7C(void) {
    s32 slot = (u8)func_801E8D0C_13AC2C(D_801F51E8_147108->args[0].word, 0);
    OvlBUnitHead *unit = func_800A8CB0(slot);
    OvlBUnitHead *target;
    u8 id;

    if (func_800A8974(unit)) {
        return;
    }
    id = func_800A8C00(unit);
    OvlBPos from(unit->pos);
    OvlBPos to;
    target = unit;

    switch (D_801F5CCD_147BED) {
    case 0x60:
        func_801E6B4C_138A6C(slot);
        break;
    case 0x61: {
        if (D_801F51E8_147108->args[3].word & 1) {
            func_801EB6A0_13D5C0(1);
            to.y = from.y + D_801F51E8_147108->args[1].word;
        } else {
            to.y = D_801F51E8_147108->args[1].word;
        }
        if (D_801F51E8_147108->args[3].word & 2) {
            func_801EB6A0_13D5C0(2);
            to.x = from.x + D_801F51E8_147108->args[2].word;
        } else {
            to.x = D_801F51E8_147108->args[2].word;
        }
        func_801E61B8_1380D8(slot, 0, 0, 0);
        target->unk52 = 4;
        func_800A58FC(target, &to);
        func_801E7200_139120(id, from.y, from.x, to.y, to.x);
        func_801F33FC_14531C();
        break;
    }
    case 0x62:
        if (D_801F51E8_147108->args[1].word == 2) {
            func_801E7610_139530(id, (u8)D_801F51E8_147108->args[2].word, (u8)(D_801F51E8_147108->args[2].word >> 8), D_801F51E8_147108->args[3].word);
        }
        break;
    case 0x63: {
        func_801F34D8_1453F8(unit, 0);
        func_801E70D8_138FF8(id);
        OvlBDir dir(D_801F51E8_147108->args[1].word);
        func_800A665C(unit, &dir);
        break;
    }
    case 0x67:
        func_801E70D8_138FF8(id);
        func_801F34D8_1453F8(unit, 1);
        target = unit_of(D_801F51E8_147108->args[1].word);
        if (!func_800A8974(target)) {
            func_801F349C_1453BC(unit, target);
            func_801F33FC_14531C();
        }
        break;
    case 0x68:
        func_801F34D8_1453F8(unit, 1);
        func_801E70D8_138FF8(id);
        target = unit_of(D_801F51E8_147108->args[1].word);
        if (!func_800A8974(target)) {
            s32 facing = unit->facing.v;
            s32 angle = func_801F49BC_1468DC(unit, target);
            if (D_801F51E8_147108->args[2].word == 0) {
                OvlBDir dir(angle);
                func_800A665C(unit, &dir);
            } else {
                func_801E7074_138F94(slot, facing, angle, D_801F51E8_147108->args[2].word);
            }
        }
        break;
    case 0x69:
        func_801E6A8C_1389AC(id);
        func_800E039C(target, (u8)D_801F51E8_147108->args[1].word);
        func_80049CB4(0x1F, target);
        break;
    case 0x6A:
        func_801E61B8_1380D8(slot, D_801F51E8_147108->args[1].word, D_801F51E8_147108->args[2].word, D_801F51E8_147108->args[3].word);
        break;
    case 0x6B:
        func_801E6394_1382B4(slot, 1);
        func_801E6708_138628(slot, D_801F51E8_147108->args[1].word, unit->facing.v, unit->unk0A);
        break;
    case 0x6C: {
        s32 dy = SCRIPT_BYTE();
        s32 dx = SCRIPT_BYTE();
        s32 model;
        if (dy & 0x80) {
            dy |= ~0xFF;
        }
        if (dx & 0x80) {
            dx |= ~0xFF;
        }
        model = SCRIPT_BYTE();
        model = (model << 8) + SCRIPT_BYTE();
        func_801E72BC_1391DC(slot, D_801F51E8_147108->args[1].word, D_801F51E8_147108->args[3].word, dy, dx, D_801F51E8_147108->args[2].word, model);
        break;
    }
    case 0x6D: {
        f32 at[3];
        s32 effect;
        func_801E6394_1382B4(slot, 1);
        effect = SCRIPT_BYTE();
        /* at[1] is left unwritten here (no store to sp+0x34 on this path). */
        at[0] = from.y;
        at[2] = from.x;
        if (D_801F51E8_147108->args[3].word != 0xFF) {
            func_801E6D08_138C28(D_801F51E8_147108->args[1].word, D_801F51E8_147108->args[3].word + 100, effect, D_801F51E8_147108->args[2].word, at, slot);
        } else {
            func_801E6D08_138C28(D_801F51E8_147108->args[1].word, (unit->facing.v + 4) % 8, effect, D_801F51E8_147108->args[2].word, at, slot);
        }
        break;
    }
    case 0x6E: {
        func_801E6A8C_1389AC(slot);
        func_801F34D8_1453F8(unit, 1);
        if (D_801F51E8_147108->args[3].word & 1) {
            func_801EB6A0_13D5C0(1);
            to.y = from.y + D_801F51E8_147108->args[1].word;
        } else {
            to.y = D_801F51E8_147108->args[1].word;
        }
        if (D_801F51E8_147108->args[3].word & 2) {
            func_801EB6A0_13D5C0(2);
            to.x = from.x + D_801F51E8_147108->args[2].word;
        } else {
            to.x = D_801F51E8_147108->args[2].word;
        }
        if (from.y != to.y || from.x != to.x) {
            func_800E5F38(target, &to);
        }
        break;
    }
    case 0x6F:
        func_801E75CC_1394EC(slot);
        break;
    case 0x70:
        func_801E747C_13939C(slot);
        break;
    case 0x71: {
        s32 mode;
        switch (D_801F51E8_147108->args[1].word) {
        case 0:
            mode = 3;
            break;
        case 1:
            mode = 4;
            break;
        default:
            mode = 9;
            break;
        }
        func_801EB6A0_13D5C0(2);
        func_801E7AA0_1399C0(unit->unk0A, mode, D_801F51E8_147108->args[2].word);
        break;
    }
    case 0x72:
        func_801E6888_1387A8(id, D_801F51E8_147108->args[1].word, D_801F51E8_147108->args[2].word, D_801F51E8_147108->args[3].word, SCRIPT_BYTE());
        break;
    case 0x76:
        func_801EB6A0_13D5C0(1);
        func_801E65EC_13850C(id, D_801F51E8_147108->args[1].word, unit->unk0A);
        break;
    case 0x77: {
        f32 at[3];
        s32 model = SCRIPT_BYTE();
        s32 a;
        s32 b;
        s32 c;
        s32 d;
        model = (model << 8) + SCRIPT_BYTE();
        a = SCRIPT_BYTE();
        b = SCRIPT_BYTE();
        c = SCRIPT_BYTE();
        d = SCRIPT_BYTE();
        func_801EB6A0_13D5C0(1);
        func_801EB6A0_13D5C0(2);
        func_801EB6A0_13D5C0(3);
        at[0] = D_801F51E8_147108->args[1].word;
        at[1] = D_801F51E8_147108->args[2].word;
        at[2] = D_801F51E8_147108->args[3].word;
        func_801E7C88_139BA8(model, a, id, d, b, at);
        func_801E6394_1382B4(slot, c);
        break;
    }
    case 0x78: {
        func_801E6A8C_1389AC(slot);
        if (D_801F51E8_147108->args[3].word & 1) {
            func_801EB6A0_13D5C0(1);
            to.y = from.y + D_801F51E8_147108->args[1].word;
        } else {
            to.y = D_801F51E8_147108->args[1].word;
        }
        if (D_801F51E8_147108->args[3].word & 2) {
            func_801EB6A0_13D5C0(2);
            to.x = from.x + D_801F51E8_147108->args[2].word;
        } else {
            to.x = D_801F51E8_147108->args[2].word;
        }
        OvlBDir dir(SCRIPT_BYTE());
        func_800A665C(target, &dir);
        if (from.y != to.y || from.x != to.x) {
            func_800E5F5C(target, &to);
        }
        break;
    }
    case 0x79: {
        func_801F34D8_1453F8(unit, 1);
        func_801E70D8_138FF8(id);
        s32 facing = unit->facing.v;
        s32 dir = D_801F51E8_147108->args[1].word;
        if (D_801F51E8_147108->args[2].word == 0) {
            OvlBDir d(dir);
            func_800A665C(unit, &d);
        } else {
            func_801E7074_138F94(slot, facing, dir, D_801F51E8_147108->args[2].word);
        }
        break;
    }
    case 0x7A:
        func_801F2E8C_144DAC(unit);
        break;
    case 0x7D: {
        to.y = 0;
        to.x = 0;
        func_800A58FC(unit, &to);
        func_80049CB4(0x87, unit);
        func_800A59A4(unit);
        func_80049CB4(2);
        break;
    }
    case 0x7B: {
        Rec16 desc;
        desc.unk0C = D_801F51E8_147108->args[1].word;
        desc.unk0E = D_801F51E8_147108->args[2].word;
        desc.coordinates.bytes.y = from.y;
        desc.coordinates.bytes.x = from.x;
        desc.unk10[0] = SCRIPT_BYTE();
        desc.unk10[1] = SCRIPT_BYTE();
        desc.unk10[2] = SCRIPT_BYTE();
        desc.unk10[3] = SCRIPT_BYTE();
        desc.unk10[4] = SCRIPT_BYTE();
        desc.unk04 = unit->facing.v;
        func_801F1B58_143A78(unit, &desc);
        break;
    }
    }
    func_80049CB4(2);
}

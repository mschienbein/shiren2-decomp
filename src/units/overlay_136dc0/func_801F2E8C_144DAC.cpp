#include "func_801F2E8C_144DAC/types.h"

extern "C" {
void *func_801EA2D8_13C1F8(s32 kind);
void *func_800CF058(void *table, u8 index);
OvlBVirtualItem *func_800AC244(u8 index);
s32 func_800AFBFC(OvlBVirtualItem *item);
void func_801EB6A0_13D5C0(u8 arg0);
s32 func_800D818C(s32 arg0);
s32 func_800D8144(s32 arg0);
}

extern "C" void func_801F2E8C_144DAC(OvlBUnitHead *unit) {
    OvlBPos pos = unit->pos;
    OvlBVirtualItem *item;
    OvlBUnitHead *target;

    switch (D_801F51E8_147108->args[1].word) {
    case 133: {
        s32 message;
        void *table;
        void *entry;

        if (!((unit->unk1E >> 2) & 1)) {
            break;
        }
        message = 0x85;
        if (D_801F51E8_147108->args[2].word != 0) {
            message = 0x84;
        }
        table = func_801EA2D8_13C1F8(unit->unk0A);
        if (table == NULL) {
            break;
        }
        entry = func_800CF058(table, 9);
        if (entry == NULL) {
            break;
        }
        func_80049CB4(message, unit, entry);
        break;
    }
    case 256:
        func_80049CB4(0xA);
        target = func_800A8CB0((u8)func_801E8D0C_13AC2C((u16)D_801F51E8_147108->args[2].word, 0));
        if (func_800A8974(target)) {
            break;
        }
        if (func_800A8974(unit)) {
            break;
        }
        {
            OvlBPos other = target->pos;
            func_80049CB4(D_801F51E8_147108->args[1].word, &pos, &other);
        }
        break;
    case 247:
        func_80049CB4(0xF7, &pos, D_801F51E8_147108->args[2].word);
        break;
    case 185:
        target = func_800A8CB0((u8)func_801E8D0C_13AC2C((u16)D_801F51E8_147108->args[2].word, 0));
        if (func_800A8974(target)) {
            break;
        }
        {
            OvlBPos other = target->pos;
            item = func_800AC244(D_801F51E8_147108->args[3].word);
            if (func_800AFBFC(item)) {
                break;
            }
            func_80049CB4(D_801F51E8_147108->args[1].word, item, &pos, &other);
        }
        delete item;
        break;
    case 269:
    case 272:
        func_801EB6A0_13D5C0(2);
        func_801EB6A0_13D5C0(3);
        {
            OvlBPos other;
            other.y = pos.y + D_801F51E8_147108->args[2].word;
            other.x = pos.x + D_801F51E8_147108->args[3].word;
            func_80049CB4(D_801F51E8_147108->args[1].word, &pos, &other);
        }
        break;
    case 267:
        func_801EB6A0_13D5C0(2);
        func_801EB6A0_13D5C0(3);
        {
            OvlBPos other;
            other.y = pos.y + D_801F51E8_147108->args[2].word;
            other.x = pos.x + D_801F51E8_147108->args[3].word;
            func_80049CB4(D_801F51E8_147108->args[1].word, &other);
        }
        break;
    case 112:
        func_80049CB4(0x70, unit);
        break;
    case 22:
        item = func_800AC244(D_801F51E8_147108->args[3].word);
        if (func_800AFBFC(item)) {
            break;
        }
        func_80049CB4(0x70, unit);
        delete item;
        break;
    case 144: {
        u8 kind = unit->unk0A;

        if (kind != 41) {
            target = func_800A8CB0((u8)func_801E8D0C_13AC2C(kind, (u16)D_801F51E8_147108->args[2].word));
        } else {
            target = func_800A8CB0((u8)func_801E8D0C_13AC2C(41, (u16)func_800D8144(func_800D818C(D_801F51E8_147108->args[2].word))));
        }
        if (func_800A8974(target)) {
            break;
        }
        func_80049CB4(0x90, target);
        func_80049CB4(2);
        break;
    }
    }
    func_80049CB4(0xD);
    func_80049CB4(2);
    func_80049CB4(0xE);
}

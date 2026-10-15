#include "func_801F1B58_143A78/types.h"

extern "C" {
void func_801F2B20_144A40(Part *part, u16 a, u16 b, u16 c, u16 d);
void func_801F1F64_143E84(OvlBUnitHead *unit);
}

/* Applies a part settings record to whichever Part component the unit variant carries,
 * then refreshes the unit (func_801F1F64_143E84). */
extern "C" void func_801F1B58_143A78(OvlBUnitHead *unit, PartSpec *spec) {
    if ((unit->unk1E >> 6) & 1) {
        UnitPart78 *owner = (UnitPart78 *)unit;
        owner->setField18(spec->f0C);
        owner->setField2C(spec->f14);
        s32 mode = spec->f0E & 3;
        switch (mode) {
        case 0:
            owner->setMode(0);
            break;
        case 2:
            func_801F2B20_144A40(owner, spec->f10, spec->f11, spec->f12, spec->f13);
            owner->setMode(2);
            break;
        case 1:
            owner->setMode(1);
            break;
        default:
            func_801F2B20_144A40(owner, spec->f10, spec->f11, spec->f12, spec->f13);
            owner->setMode(3);
            break;
        }
        switch (spec->f0E & 0x30) {
        case 0: owner->setField14(1); break;
        case 0x30: owner->setField14(2); break;
        default: owner->setField14(0); break;
        }
        if (spec->f0E & 0x40) owner->setField0C(0);
        else owner->setField0C(1);
        owner->setField08((spec->f0E >> 7) & 1);
        if (spec->f0E & 8) owner->setField10(1);
        u8 bits = spec->f04 & 7;
        owner->setField2D(bits);
    }
    if ((unit->unk1E >> 3) & 1) {
        UnitPart84 *owner = (UnitPart84 *)unit;
        owner->setField18(spec->f0C);
        owner->setField2C(spec->f14);
        s32 mode = spec->f0E & 3;
        switch (mode) {
        case 0:
            owner->setMode(0);
            break;
        case 2:
            func_801F2B20_144A40(owner, spec->f10, spec->f11, spec->f12, spec->f13);
            owner->setMode(2);
            break;
        case 1:
            owner->setMode(1);
            break;
        default:
            func_801F2B20_144A40(owner, spec->f10, spec->f11, spec->f12, spec->f13);
            owner->setMode(3);
            break;
        }
        switch (spec->f0E & 0x30) {
        case 0: owner->setField14(1); break;
        case 0x30: owner->setField14(2); break;
        default: owner->setField14(0); break;
        }
        if (spec->f0E & 0x40) owner->setField0C(0);
        else owner->setField0C(1);
        owner->setField08((spec->f0E >> 7) & 1);
        if (spec->f0E & 8) owner->setField10(1);
        u8 bits = spec->f04 & 7;
        owner->setField2D(bits);
    }
    func_801F1F64_143E84(unit);
}

#include "func_801EDFBC_13FEDC/types.h"

extern void func_80045A24(s32);
extern void func_80045A60(void);
extern void func_80045A84(void);
/* ROM 0x140214/0x13FFE8 supplies word operands; the host facades
 * narrow to signed halfwords locally before their downstream calls. */
extern void func_80045AC8(s32);
extern void func_80045BC0(u8, u8);
extern void func_80045BE0(u8, u8);
extern void func_80045C00(s32, u8, u8);
extern void func_80045C68(u8);
extern void func_80045C84(s32);
extern void func_80046C30(s32);
extern void func_800485C0(void);
/* 801EE038..044 supplies the player even though the current facade ignores it. */
extern void func_800485DC(void *);
extern void func_800485F8(void);
extern s32 func_80049CB4(s32, ...);
extern char *func_80083C90(char *, char *);
extern OvlItem *func_800AB134(void);
extern OvlItem *func_800AC244(u8);
extern void func_800ACD34(OvlItem *);
extern void func_800AD7E0(OvlItem *, void *, s32);
extern char *func_800AE674(OvlItem *);
extern void func_800B1CEC(s32, Vec2i *);
extern void func_800B4E7C(void *);
extern s32 func_800CD538(void *, OvlItem *);
extern void func_801E5168_137088(s32, s32, s32);
extern u8 func_801E53A4_1372C4(s32);
/* ROM 0x140294..0x14029C loads u16 operands; promotion preserves those loads.
 * The provider consumes duration/mode as words before byte stores. */
extern void func_801E6000_137F20(u16, s32, s32, BytePair *, u8);
extern void func_801E60B0_137FD0(BytePair *, u8, u8);
extern void func_801E63BC_1382DC(s32, OvlOperand *);
extern void func_801E7A0C_13992C(s32);
extern void func_801E7ABC_1399DC(void);
extern void func_801E7AD8_1399F8(void);
/* first is explicitly u8; promotion retains ROM 0x1401E4's byte conversion. */
extern void func_801E7E48_139D68(s32, s32, s32, s32, s32);
extern void func_801E8590_13A4B0(s32, s32, s32, s32);
extern u8 func_801E8CF4_13AC14(u8, u8);
extern s32 func_801E8D0C_13AC2C(u16, u16);
extern void *func_801EA2D8_13C1F8(s32);
extern s32 func_801EA504_13C424(s32);
extern void func_801EA600_13C520(s32, s32);
extern void func_801EA810_13C730(OvlItem *, s32);
extern void func_801EA91C_13C83C(s32, s32, s32);
extern void func_801EAAA8_13C9C8(void);
extern OvlScriptRecord *func_801EB53C_13D45C(OvlScriptRecord *);
extern OvlScriptRecord *func_801EB5D4_13D4F4(u16);
extern void func_801EB6A0_13D5C0(u8);
extern s32 func_801EC87C_13E79C(s32, s32, u8);
extern s32 func_801EDBFC_13FB1C(void);
extern void func_801EDEF8_13FE18(void *, Vec2i *);
extern void func_801F3318_145238(s32);

/* ODD_C: a real position predicate, not an identity/barrier wrapper.
 * The direct expression in edfbc-01 emitted a 2032-byte body, lacking the
 * original reload/saved pointer. This inline form plus case-local lifetimes
 * yields the 2044-byte, zero-word-difference edfbc-04 result. */
static inline s32 position_is_zero(Vec2i *position)
{
    return (position->y | position->x) == 0;
}

static inline void destroy_item(OvlItem *item)
{
    if (item) item->field08->method0C((u8 *)item + item->field08->delta08, 3);
}

u16 func_801EDFBC_13FEDC(void)
{
    union { Vec2i vec; BytePair byte; } position;
    Vec2i player;
    OvlItem *item;
    u8 first;
    s32 value;
    void *owner;

    if ((u16)func_801EDBFC_13FB1C()) return 4;
    switch (D_801F5CCD_147BED) {
    case 0x3B:
        if (D_801F51E8_147108->operand[0].word == 0) {
            func_800485F8();
        } else {
            func_800485C0();
            func_800485DC(D_801476B8);
        }
        break;
    case 0x3C:
        func_801EB6A0_13D5C0(2);
        switch (D_801F51E8_147108->operand[3].word) {
        case 0:
            func_80045BC0(D_801F51E8_147108->operand[0].byte[3], D_801F51E8_147108->operand[1].byte[3]);
            break;
        case 1:
            func_80045BE0(D_801F51E8_147108->operand[0].byte[3], D_801F51E8_147108->operand[1].byte[3]);
            break;
        case 2:
            func_80045C00(D_801F51E8_147108->operand[2].word, D_801F51E8_147108->operand[0].byte[3], D_801F51E8_147108->operand[1].byte[3]);
            break;
        case 3:
            if (D_801F51E8_147108->operand[0].word == 0)
                func_80045C84(D_801F51E8_147108->operand[1].byte[3]);
            else
                func_80045C68(D_801F51E8_147108->operand[1].byte[3]);
            break;
        }
        break;
    case 0x3D: {
        u8 notify = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        position.vec.y = D_801F51E8_147108->operand[1].word;
        position.vec.x = D_801F51E8_147108->operand[2].word;
        if (position_is_zero(&position.vec)) {
            item = func_800AC244(D_801F51E8_147108->operand[0].byte[3]);
            if (item) {
                player.x = D_801476B8->x;
                player.y = D_801476B8->y;
                func_801EDEF8_13FE18(item, &player);
            }
        } else {
            func_800B4E7C(&position.vec);
            if (D_801F51E8_147108->operand[0].word == 0) {
                func_80049CB4(0xD7, &position.vec);
            } else {
                item = func_800AC244(D_801F51E8_147108->operand[0].byte[3]);
                if (item) {
                    func_801EA810_13C730(item, D_801F51E8_147108->operand[3].word);
                    D_801F51E8_147108->item118 = item;
                    func_80083C90(D_801F51F0_147110, func_800AE674(item));
                    if (notify) {
                        func_80049CB4(0xD2, item, &position.vec);
                        func_80049CB4(0xD);
                        func_80049CB4(2);
                        func_80049CB4(0xE);
                    }
                    if (item->field01 == 0x8A) func_800ACD34(item);
                    func_800AD7E0(item, &position.vec, 1);
                    func_800B1CEC(1, &position.vec);
                    func_80049CB4(2);
                }
            }
        }
        break;
    }
    case 0x3E:
        first = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        func_801E7E48_139D68(first, D_801F51E8_147108->operand[0].word, D_801F51E8_147108->operand[1].word, D_801F51E8_147108->operand[2].word, D_801F51E8_147108->operand[3].word);
        break;
    case 0x3F:
        if (D_801F51E8_147108->operand[1].word)
            func_80045A24(D_801F51E8_147108->operand[0].word);
        else
            func_80045AC8(D_801F51E8_147108->operand[0].word);
        break;
    case 0x40: {
        u8 first = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        u8 second = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        func_801EB6A0_13D5C0(0);
        func_801E60B0_137FD0(&position.byte, D_801F51E8_147108->operand[3].byte[3], first);
        func_801E6000_137F20(D_801F51E8_147108->operand[0].half[1], D_801F51E8_147108->operand[1].half[1], D_801F51E8_147108->operand[2].half[1], &position.byte, second);
        break;
    }
    case 0x41:
        first = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        if (first == 1) func_801E7AD8_1399F8();
        else if (first == 0) func_801E7ABC_1399DC();
        else func_801E8590_13A4B0(D_801F51E8_147108->operand[0].word, D_801F51E8_147108->operand[1].word, D_801F51E8_147108->operand[2].word, D_801F51E8_147108->operand[3].word);
        break;
    case 0x42:
        func_801EB6A0_13D5C0(2);
        func_801EA91C_13C83C(D_801F51E8_147108->operand[0].word, D_801F51E8_147108->operand[1].word, D_801F51E8_147108->operand[2].word);
        break;
    case 0x43:
        func_801EA600_13C520(D_801F51E8_147108->operand[0].word, D_801F51E8_147108->operand[1].word);
        break;
    case 0x44:
        func_801EA504_13C424(D_801F51E8_147108->operand[0].word);
        break;
    case 0x45:
        if (D_801F51E8_147108->operand[0].word) {
            func_80045A60();
            D_801F51E8_147108->field9F9 = 0;
        } else {
            func_80045A84();
            D_801F51E8_147108->field9F9 = 1;
        }
        break;
    case 0x46:
        value = D_801F51E8_147108->operand[1].word;
        if (value == 2) {
            func_801E5168_137088(0, 0, D_801F51E8_147108->operand[0].word);
        } else {
            s32 channel;
            func_801E5168_137088(1, value * 255, D_801F51E8_147108->operand[0].word);
            channel = D_801F51E8_147108->operand[2].word;
            if (channel) func_801E7A0C_13992C(channel - 1);
        }
        break;
    case 0x47:
        func_801EB6A0_13D5C0(3);
        first = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        return (u16)func_801EC87C_13E79C(D_801F51E8_147108->operand[0].word, D_801F51E8_147108->operand[3].word, first);
    case 0x48:
        func_801F3318_145238(D_801F51E8_147108->operand[0].word);
        break;
    case 0x49: {
        u8 first = D_801F51E8_147108->operand[0].byte[3];
        u8 second = func_801E8D0C_13AC2C(D_801F51E8_147108->operand[0].half[1], 0);
        D_801F51E8_147108->operand[0].word = func_801E53A4_1372C4(D_801F51EC_14710C->field4++);
        func_801E63BC_1382DC(second, D_801F51E8_147108->operand);
        D_801F51E8_147108->operand[0].word = first;
        break;
    }
    case 0x4A:
        D_801F51E8_147108->field999 = D_801F51E8_147108->operand[0].byte[3];
        break;
    case 0x4B: {
        OvlItem *item = func_800AB134();
        D_801F51E8_147108->item118 = item;
        if (item) {
            func_80083C90(D_801F51F0_147110, func_800AE674(item));
            func_801E8CF4_13AC14(4, 0);
            owner = func_801EA2D8_13C1F8(0x17);
            if (!owner) {
                destroy_item(D_801F51E8_147108->item118);
            } else if (func_800CD538(owner, D_801F51E8_147108->item118)) {
                func_801E8CF4_13AC14(4, 2);
            } else {
                destroy_item(D_801F51E8_147108->item118);
                func_801E8CF4_13AC14(4, 1);
                D_801F51E8_147108->item118 = 0;
            }
        }
        break;
    }
    case 0x4F:
        func_801EB6A0_13D5C0(3);
        func_801EAAA8_13C9C8();
        break;
    case 0x4C:
        func_801EB6A0_13D5C0(1);
        D_801F5CCC_147BEC += D_801F51E8_147108->operand[1].byte[3];
        if (D_801F5CCC_147BEC < 0) D_801F5CCC_147BEC = 0;
        else if (D_801F5CCC_147BEC >= 8) D_801F5CCC_147BEC = 7;
        D_801F51EC_14710C = func_801EB5D4_13D4F4(D_801F51E8_147108->operand[0].half[1]);
        break;
    case 0x4D:
        func_80046C30(0);
        func_80049CB4(0xB, D_801F51E8_147108->operand[0].word);
        break;
    case 0x4E:
        D_801F51EC_14710C = func_801EB53C_13D45C(D_801F51EC_14710C);
        break;
    }
    return 4;
}

#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;

typedef struct {
    u8 id;
    u8 unk1;
    u8 unk2;
    u8 skills[16];
} Member8009E724;

typedef struct {
    u8 id;
    u8 flags;
} Equip8009E724;

typedef struct {
    u8 pad0[0xD];
    u8 unkD;
    u8 padE;
    s8 unkF;
} Unit8009E724;

/* The 0x68-byte record at Save+0x54 is consumed by func_800CC728/74C/7EC. */
typedef struct {
    u8 pad0[3];
    u8 unk3;
    u8 name4[8];
    u32 unkC;
    u32 playTime;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    char entries[0x67 - 0x17];
    u8 entryCount;
} Record8009E724;

typedef struct {
    u8 pad0[0x38];
    s32 unk38;
    u8 pad3C[0x51 - 0x3C];
    u8 unk51;
    u8 pad52[2];
    Record8009E724 record54;
    u16 unkBC;
    u8 padBE[2];
    s32 unkC0;
    Member8009E724 members[2];
    Equip8009E724 equips[2];
    u8 padEE[0xF0 - 0xEE];
    s32 unkF0;
} Save8009E724;

char *func_80048480(u16 textId);
void func_80048764(Save8009E724 *self);
void func_800487EC(Save8009E724 *self, s32 row, s32 col, char *text);
void func_80048870(Save8009E724 *self, s32 color);
char *func_800514F0(Unit8009E724 *unit, char *buf, s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_8005EF30(char *buf, const char *fmt, ...);
char *func_80083C90(char *dst, char *src);
char *func_80083D04(char *dst, char *src);
char *func_80083F34(void *src, s32 len, char *dst);
void func_8009DE8C(Save8009E724 *self);
Unit8009E724 *func_800AC3CC(u8 id, void *storage);
void func_800ACD34(Unit8009E724 *unit);
void func_800AE55C(Unit8009E724 *unit, s32 arg);
char *func_800CC728(void *data);
char *func_800CC74C(void *data, u8 index);
char *func_800CC7EC(void *data);
void func_8010BC98(Unit8009E724 *unit, u8 arg);
s32 func_8010BD00(Unit8009E724 *unit, u8 skill);
char *func_8010C60C(Unit8009E724 *unit, char *buf, u8 arg);
void func_801138BC(Unit8009E724 *unit);

void func_8009E724(Save8009E724 *self) {
    char line[0x80];
    char text[0x40];
    char part[0x40];
    u8 storage[0x30];
    s32 pages = self->unk38 / self->unk51;
    s32 n = 0;
    s32 i;

    func_80048764(self);
    func_80048870(self, 0x78000000);
    if (pages == 0) {
        u32 seconds;
        u32 value;
        Record8009E724 *data;
        char *out;

        func_800487EC(self, n, n, func_80048480(0x52D));
        line[0] = 0;
        *func_80083F34(self->record54.name4, 4, part) = 0;
        func_8005EF30(text, func_80048480(0x52E), part);
        func_80083D04(line, text);
        func_8005EF30(text, func_80048480(0x52F), self->record54.unk3);
        func_80083D04(line, text);
        func_8005EF30(text, func_80048480(0x530), self->record54.unkC);
        func_80083D04(line, text);
        out = line;
        func_800487EC(self, n, n, out);
        data = &self->record54;
        {
            char *name = func_800CC728(data);
            func_8005EF30(out, func_80048480(0x532), name);
        }
        func_800487EC(self, 1, n, out);
        seconds = self->record54.playTime;
        n = 0x1F;
        text[0] = 0;
        value = seconds / 3600;
        if (value != 0) {
            seconds -= value * 3600;
            func_8005EF30(part, func_80048480(0x539), value);
            func_80083D04(text, part);
            n = 0x5D;
        }
        value = seconds / 60;
        if (value != 0) {
            seconds -= value * 60;
            func_8005EF30(part, func_80048480(0x53A), value);
            func_80083D04(text, part);
            n += 0x1F;
        }
        func_8005EF30(part, func_80048480(0x53B), seconds);
        func_80083D04(text, part);
        func_8005EF30(out, func_80048480(0x533), 0xF0 - n, text);
        func_800487EC(self, 1, 0, out);
        func_80083C90(out, func_800CC7EC(data));
        func_800487EC(self, 2, 0, out);
        if (self->unkF0 == 0) {
            goto done;
        }
        func_8005EF30(out, func_80048480(0x534), self->unkBC);
        func_800487EC(self, 4, 0, out);
        func_8005EF30(out, func_80048480(0x535), self->record54.unk14);
        func_800487EC(self, 4, 0, out);
        func_8005EF30(out, func_80048480(0x536), self->record54.unk15, self->record54.unk16);
        func_800487EC(self, 5, 0, out);
        func_8005EF30(out, func_80048480(0x537), self->unkC0);
        func_800487EC(self, 5, 0, out);
        n = 6;
        i = 2;
        for (;;) {
            Member8009E724 *member;
            Unit8009E724 *unit;
            s32 j;

            i--;
            if (i == -1) {
                break;
            }
            member = (i != 0) ? &self->members[0] : &self->members[1];
            if (member->id != 0) {
            unit = func_800AC3CC(member->id, storage);
            func_800ACD34(unit);
            unit->unkD = member->unk1;
            func_8010BC98(unit, member->unk2);
            for (j = 0; j < 16; j++) {
                if (member->skills[j] == 0) {
                    break;
                }
                func_8010BD00(unit, member->skills[j]);
            }
            if (unit->unkF != 0) {
                func_80048870(self, 0x28000000);
            }
            func_800514F0(unit, line, 0, 0, -1, 0, 0);
            func_800487EC(self, n, 0, out);
            n++;
            func_8010C60C(unit, out, 0);
            func_800487EC(self, n, 0, out);
            n++;
            func_80048870(self, 0x78000000);
            } else {
                n += 2;
            }
        }
        i = 0;
        for (;;) {
            Equip8009E724 *equip;

            if (i >= 2) {
                break;
            }
equip = &self->equips[i];
            if (equip->id != 0) {
                Unit8009E724 *unit = func_800AC3CC(equip->id, storage);
                Unit8009E724 *base = unit;

                func_800ACD34(unit);
                if (equip->flags & 1) {
                    func_801138BC(unit);
                }
                if ((equip->flags >> 1) & 1) {
                    func_800AE55C(unit, 1);
                }
                func_800514F0(base, line, 0, 0, -1, 0, 0);
                func_800487EC(self, n, 0, line);
            }            n++;
            i++;
        }
    } else {
        s32 first = n++;

        func_800487EC(self, first, 0, func_80048480(0x538));
        n++;
        i = 0;
        for (;;) {
            char *entry;

            if (!(i < self->record54.entryCount && i < 10)) {
                break;
            }
            entry = func_800CC74C(&self->record54, i);
            func_80083C90(line, entry);
            func_800487EC(self, n + i, 0, line);
            i++;
        }
        func_80083C90(line, func_800CC7EC(&self->record54));
        func_800487EC(self, n + i, 0, line);
    }
done:
    func_8009DE8C(self);
}

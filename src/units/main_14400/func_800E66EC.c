#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct { s32 unk0, unk4; } Pair;
typedef struct { u8 value; } Dir;
typedef struct { u32 unk0 : 5; u32 unk5 : 1; u32 unk6 : 26; } Flags;
typedef struct State State;
typedef struct { char unk0[0x10]; s16 unk10, unk12; s32 (*unk14)(void *); } Dispatch;
struct State {
    Pair position;
    Dir unk8;
    u8 unk9;
    char unkA[0x14]; u8 unk1E; char unk1F;
    Flags unk20; Dispatch *unk24;
    char unk28[0x2C]; u8 unk54; char unk55[3]; State *unk58;
    Pair unk5C, unk64;
    char unk6C[0xA]; u8 unk76; char unk77[0x23]; u8 unk9A;
};
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern u8 D_8014344C;
extern u8 D_80147620[];
extern const s8 D_80148340[8], D_80148348[4], D_8014834C[4];
extern Dir *func_800A22B8(Dir *, Pair *, Pair *);
extern s32 func_800A23E8(Pair *, Pair *);
extern s32 func_800A251C(Pair *, Pair *);
extern void *func_800A2594(Pair *, Pair *, Dir);
extern Pair *func_800A25D8(Pair *, Pair *, Dir, s32);
extern void func_800A2F80(u8 *, s32);
extern void func_800A2F94(u8 *, s32);
extern s32 func_800A41EC(State *, Pair *);
extern s32 func_800A44F4(State *, State *);
extern s32 func_800A455C(State *, State *, s32);
extern s32 func_800A4754(State *, Pair *, Dir *);
extern s32 func_800A4CC4(State *, Pair *, Dir *);
extern s32 func_800A4EFC(State *, void *);
extern s32 func_800A5018(State *, s32);
extern s32 func_800A50AC(State *);
extern s32 func_800A50E8(State *);
extern void func_800A665C(State *, u8 *);
extern char *func_800A7DE4(State *);
extern u32 func_800B1C6C(Pair *);
extern void *func_800B1F90(Pair *);
extern void *func_800B4928(Pair *);
extern s32 func_800B56F0(Pair *);
extern s32 func_800B68B0(void *);
extern s32 func_800B69F4(void *, Pair *);
extern void *func_800B6A98(Pair *, void *, s32);
extern void *func_800B6BA0(Pair *, void *);
extern Pair *func_800B6C14(Pair *, void *, s32);
extern u8 func_800C57A0(void *);
extern u8 func_800C57CC(void *, s32);
extern s32 func_800E1CC4(State *, s32);
extern s32 func_800E65C0(State *, Pair *, s32);

static inline void copyPair(Pair *dst, Pair *src) {
    dst->unk0 = src->unk0;
    dst->unk4 = src->unk4;
}

/* Flags is a memory copy of the +0x20 status word; bit 26 marks a blocked lookahead. */
static inline s32 flag26(Flags *flags) {
    return flags->unk5;
}

static inline s32 isBusy(State *self) {
    return self->unk9A & 1;
}

static inline s32 inRoom(Pair *position) {
    return func_800B1C6C(position) & 0x1000;
}

static inline s32 turnOffset(s32 index) {
    return D_8014834C[index];
}

static inline s32 sideOffset(s32 index) {
    return D_80148348[index];
}

static inline Dir *setDir(Dir *dir, s32 value) {
    dir->value = value & 7;
    return dir;
}

static inline void faceLeftOf(State *self, Dir *base) {
    Dir turned;
    setDir(&turned, base->value - 1);
    func_800A665C(self, &turned.value);
}

s32 func_800E66EC(State *arg0) {
    Pair current, destination, next, adjacent, second;
    Dir towards;
    Dir direction;
    Dir candidate1, candidate2, candidate3;

    copyPair(&current, &arg0->position);
    copyPair(&destination, &arg0->unk64);
    direction = arg0->unk8;
    if (destination.unk4 | destination.unk0) {
        s32 reached = 0;
        if (func_800A251C(&destination, &current) || ((func_800B1C6C(&current) & 0x800) && (func_800B1C6C(&destination) & 0x800))) reached = 1;
        if (reached) {
            destination.unk0 = 0;
            destination.unk4 = 0;
            arg0->unk64.unk0 = 0;
            arg0->unk64.unk4 = 0;
        } else {
            func_800A22B8(&towards, &current, &destination);
            func_800A665C(arg0, &towards.value);
        }
    }
    {
        s32 blocked = 0;
        if (D_8014344C == 1) {
            Flags flags = arg0->unk20;
            if (flag26(&flags)) {
                func_800A2594(&next, &current, arg0->unk8);
                blocked = func_800B56F0(&next) != 0;
            }
            if (blocked) return 0;
            {
                s32 pending = func_800A50AC(arg0) ^ 1;
                if (pending) {
                    pending = func_800A50E8(arg0) ^ 1;
                    if (pending) {
                        Dir base;
                        faceLeftOf(arg0, setDir(&base, arg0->unk8.value + func_800C57CC(D_80147620, 2)));
                    }
                }
            }
            return 1;
        }
    }
    if (arg0->unk76 >= 11) {
        if (inRoom(&current)) {
            void *room = func_800B1F90(&current);
            s32 exit = func_800B69F4(room, &arg0->unk64);
            if (exit < 0) {
                func_800B6BA0(&next, room);
                destination = next;
            } else {
                func_800B6C14(&next, room, exit);
                destination = next;
            }
        } else {
            if (func_800A50E8(arg0)) {
                arg0->unk76 = 0;
                return 1;
            }
            destination.unk0 = 0;
            destination.unk4 = 0;
        }
        arg0->unk64 = destination;
    }
    if (destination.unk4 | destination.unk0) {
        if (func_800E65C0(arg0, &destination, 1)) return 1;
    } else {
        if (inRoom(&current)) {
            void *room = func_800B1F90(&current);
            copyPair(&next, &arg0->unk5C);
            switch (func_800B68B0(room)) {
            case 0:
                return 0;
            case 2:
                func_800B6A98(&adjacent, room, 1);
                destination = adjacent;
                {
                    s32 moved = func_800A251C(&next, &destination) ^ 1;
                    if (moved) break;
                }
                /* fallthrough */
            case 1:
                func_800B6A98(&adjacent, room, 0);
                destination = adjacent;
                break;
            default: {
                s32 exit = func_800B69F4(room, &next);
                if (exit < 0) {
                    func_800B6BA0(&adjacent, room);
                    destination = adjacent;
                } else {
                    s32 i;
                    if (((D_80142F18.mode & 0xE0) ^ 0x20) != 0) {
                        func_800B6C14(&adjacent, room, exit);
                        destination = adjacent;
                        if (func_800B69F4(room, &destination) != exit) break;
                    }
                    i = 0;
                    for (;;) {
                        s32 in_range = i < 8;
                        if (!in_range) break;
                        func_800B6BA0(&adjacent, room);
                        destination = adjacent;
                        adjacent.unk0 = destination.unk0;
                        adjacent.unk4 = destination.unk4;
                        if (func_800A23E8(&next, &adjacent) >= 2) break;
                        i++;
                    }
                }
                break;
            }
            }
            arg0->unk64 = destination;
            if (func_800E65C0(arg0, &destination, 1)) return 1;
        } else {
            s32 active = 0;
            if (func_800B1C6C(&current) & 0x2180) active = ((arg0->unk9 & 15) ^ 1) != 0;
            if (active) {
                if (func_800A50AC(arg0)) return 1;
                return func_800A50E8(arg0);
            }
            {
                /* heading names the facing for the turn-table lookups. */
                Dir *heading = &direction;
                if (heading->value & 1) {
                    s32 random;
                    if (func_800A4EFC(arg0, func_800A7DE4(arg0))) return 1;
                    random = func_800C57A0(D_80147620) & 1;
                    if (func_800A4CC4(arg0, &current, setDir(&candidate1, heading->value + turnOffset(random + 1)))) {
                        func_800A2F80(&heading->value, 1);
                    } else {
                        if (!func_800A4CC4(arg0, &current, setDir(&candidate2, heading->value - turnOffset(random)))) {
                            if (func_800A5018(arg0, func_800C57A0(D_80147620) & 1)) return 1;
                        } else {
                            func_800A2F94(&heading->value, 1);
                        }
                    }
                    func_800A665C(arg0, &direction.value);
                } else {
                    func_800A2594(&next, &current, direction);
                    if (func_800A41EC(arg0, &next)) {
                        switch (func_800C57A0(D_80147620) & 3) {
                        case 0:
                            func_800A2F80(&heading->value, 4);
                            /* fallthrough */
                        case 1:
                            func_800A2F94(&direction.value, 2);
                            if (func_800A4CC4(arg0, &current, &direction)) {
                                func_800A25D8(&adjacent, &current, direction, 2);
                                if (func_800A41EC(arg0, &adjacent)) {
                                    s32 open = 0;
                                    if (!(func_800B1C6C(&adjacent) & 0x2180)) open = ((arg0->unk9 & 15) ^ 1) != 0;
                                    if (open) arg0->unk64 = adjacent;
                                    return func_800A4EFC(arg0, &direction.value);
                                }
                            }
                        }
                    }
                }
            }
            direction = arg0->unk8;
            func_800A2594(&next, &current, direction);
            {
                s32 done = 0;
                if ((func_800A4754(arg0, &next, &direction) || (func_800B1C6C(&current) & 0x800)) && func_800A4EFC(arg0, &direction.value)) done = 1;
                if (done) return 1;
            }
            if (func_800A41EC(arg0, &next)) {
                const s8 *offset = D_80148340 + (func_800C57A0(D_80147620) & 4);
                s32 i = 0;
                for (;;) {
                    s32 in_range = i < 4;
                    if (!in_range) break;
                    func_800A2F80(&direction.value, *offset++);
                    if (func_800A4CC4(arg0, &current, &direction)) {
                        s32 j;
                        s32 step;
                        func_800A2594(&adjacent, &current, direction);
                        j = 0;
                        step = 1;
                        for (;;) {
                            s32 in_range = j < 3;
                            s32 found;
                            if (!in_range) break;
                            if ((direction.value ^ 1) & 1) {
                                candidate3.value = (direction.value + sideOffset(step)) & 7;
                            } else {
                                s32 back = *(D_80148348 + step - 1);
                                candidate3.value = (direction.value + back) & 7;
                            }
                            found = 0;
                            if (func_800A4754(arg0, &adjacent, &candidate3)) {
                                func_800A2594(&second, &adjacent, candidate3);
                                found = func_800A41EC(arg0, &second) != 0;
                            }
                            step++;
                            if (found) break;
                            j++;
                        }
                        if (j < 3) return func_800A4EFC(arg0, &direction.value);
                    }
                    i++;
                }
            }
            if (func_800A50AC(arg0)) return 1;
            arg0->unk76 += 2;
        }
    }
    {
        s32 scan = 0;
        if ((arg0->unk1E >> 4) & 1) scan = !isBusy(arg0);
        if (scan) {
            s32 i = 0;
            direction = arg0->unk8;
            for (;;) {
                s32 in_range = i < 8;
                State *target;
                s32 relation;
                s32 blocked;
                if (!in_range) break;
                func_800A2594(&next, &current, direction);
                blocked = 0;
                target = func_800B4928(&next);
                relation = func_800A44F4(arg0, target);
                if (!func_800A455C(arg0, target, 1) || relation == 1
                    || (relation == 0 && ((target->unk1E >> 5) & 1))
                    || ((target->unk1E & 0x7C) && func_800E1CC4(target, 1))
                    || (((target->unk1E >> 4) & 1) && (target->unk9A & 1))
                    || target->unk24->unk14((char *)target + target->unk24->unk10)) blocked = 1;
                if (!blocked) {
                    func_800A665C(arg0, &direction.value);
                    arg0->unk58 = target;
                    arg0->unk54 |= 4;
                    return 0;
                }
                func_800A2F80(&direction.value, 1);
                i++;
            }
        }
    }
    if (arg0->unk76 >= 11) {
        Dir base;
        faceLeftOf(arg0, setDir(&base, arg0->unk8.value + func_800C57CC(D_80147620, 2)));
    }
    return 0;
}

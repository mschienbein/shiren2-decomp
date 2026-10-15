#include "common.h"
#include "selection_records.h"

typedef unsigned char u8;
typedef unsigned short u16;

extern "C" {

/* Map position: two words, copied member by member by the class's assignment. Same 8-byte
   s32 x/y layout as MapPos (map_types.h), a distinct class: MapPos keeps the implicit bitwise
   operator= (block moves), this class assigns field by field. */
struct Position_800E11C0 {
    s32 x;
    s32 y;
    Position_800E11C0 &operator=(const Position_800E11C0 &other) {
        x = other.x;
        y = other.y;
        return *this;
    }
};

/* View of the unit's status-immunity flag word (+0x20), tested one bit at a time. */
struct FlagsView_800E11C0 {
    u32 bits;
    u32 test(s32 bit) const {
        return (bits >> bit) & 1;
    }
};

/* g++ 2.8 vtable entry: { s16 delta, s16 index, pfn }. */
typedef struct {
    short delta;
    short index;
    void *pfn;
} VtblEntry_800E11C0;

/* Slot 0x94 has the same contract as in func_8011CE90. */
typedef struct {
    VtblEntry_800E11C0 unk00[12];
    short refresh_delta;        /* 0x60 */
    short refresh_index;
    void (*refresh)(void *);    /* 0x64 */
    VtblEntry_800E11C0 unk68[5];
    short react_delta;          /* 0x90 */
    short react_index;
    s32 (*react)(void *, s32, s32, u8, s32); /* 0x94 */
} ObjectVtbl;

/* Unit-record view: position at +0, the +0x1E byte and the +0x24 vtable agree with the other
   b7 unit views (UnitView_800A7204, UnitView_800C37B8); the +0x20 flag word with FlagsView_800A7204. */
typedef struct {
    Position_800E11C0 position;          /* 0x00 */
    u8 field_08;
    u8 field_09;
    u8 unk0A[0x12];
    u16 field_1C;
    u8 field_1E;
    u8 field_1F;
    u32 field_20;
    ObjectVtbl *vtbl;           /* 0x24 */
    u8 unk28[4];
    u16 field_2C;
    u8 unk2E[7];
    u8 status_turns[10];        /* 0x35 */
    u8 unk3F[3];
    u8 field_42;
    u8 field_43;
    u8 field_44;
    u8 unk45[0xB];
    u16 field_50;
    u8 unk52[0x1E];
    u8 field_70;
    u8 field_71;
    u8 field_72;
    u8 unk73[0x71];
    u16 field_E4;
} ObjectView_800E11C0;

extern SelectionRecord D_80142F18;
extern u16 D_80148274[][2];
extern u16 D_80158C6C[];
extern u8 D_80147620[];
extern ObjectView_800E11C0 *D_801476B8;
extern const u8 D_8015695F;
extern const u8 D_80156961;
extern const u8 D_80156963;
extern const u8 D_80156965;
extern const u8 D_80156967;
extern const u8 D_80156969;
extern const u8 D_8015696B;
extern const u8 D_8015696D;
extern const u8 D_8015696F;
extern const u8 D_80156971;
extern const u8 D_80156973;
extern const u8 D_80156975;
extern const u8 D_80156977;
extern const u8 D_80156979;
extern const u8 D_8015697D;
extern const u8 D_8015697F;
extern const u8 D_801569F3;
extern const u8 D_80156A81;
extern const u8 D_80156A83;
extern const u8 D_80156A89;
extern const u8 D_80156A8B;

extern s32 func_800E1CD4(ObjectView_800E11C0 *object, s32 kind);
extern s32 func_800A6E90(void *object);
extern char *func_800A3B20(ObjectView_800E11C0 *object);
extern s32 func_800A44F4(void *viewer, void *object);
extern u32 func_800E10D0(ObjectView_800E11C0 *object);
extern u32 func_800E1148(ObjectView_800E11C0 *object);
extern s32 func_800E0F40(ObjectView_800E11C0 *object);
extern s32 func_800E04D0(ObjectView_800E11C0 *object);
extern u32 func_800E110C(ObjectView_800E11C0 *object);
extern s32 func_800C5844(void *rng, u8 base, u8 top);
extern u32 func_800B1C6C(Position_800E11C0 *position);
extern void func_800E20F0(ObjectView_800E11C0 *object);
extern void func_800E03BC(ObjectView_800E11C0 *object, s32 value);
extern void func_800E4734(void *object, void *actor, s32 id);
extern void func_800E43EC(ObjectView_800E11C0 *object, u16 kind, u8 state);
extern void func_800E18C0(ObjectView_800E11C0 *object, s32 kind, u8 duration, s32 adjustment);
extern void func_800E10A4(ObjectView_800E11C0 *object, s32 kind);
extern void func_800A5A70(void *object, u32 value);
extern void func_800E10E0(ObjectView_800E11C0 *object, u8 level);
extern void func_800E1118(ObjectView_800E11C0 *object, s32 kind);
extern s32 func_800A533C(ObjectView_800E11C0 *object);
extern void func_800498E4(s32 id, ...);
extern s32 func_80049CB4(s32 action, ...);

void func_800E11C0(ObjectView_800E11C0 *object, s32 kind, u8 duration, s32 adjustment) {
    /*
     * Per-kind duration ranges { lower, upper } from the read-only item
     * parameter entries (upper 0: the lower value is used as is).  A g++
     * function-local static with dynamic initializers, filled on first use
     * under its guard word (.data 0x801482C8; the table is the .bss array at
     * 0x801C9EB0), like func_800EA3F0's table.
     */
    static u8 duration_range[21][2] = {
        { D_80156969, 0 },
        { D_8015696B, 0 },
        { D_8015696D, 0 },
        { D_801569F3, 0 },
        { D_8015697F, 0 },
        { D_8015695F, D_80156961 },
        { D_8015695F, D_80156961 },
        { D_8015695F, D_80156961 },
        { D_8015695F, D_80156961 },
        { D_80156975, 0 },
        { D_80156965, 0 },
        { D_80156967, 0 },
        { D_8015696F, 0 },
        { D_80156971, 0 },
        { D_80156973, 0 },
        { D_80156A81, D_80156A83 },
        { D_80156A89, D_80156A8B },
        { D_80156977, 0 },
        { D_80156979, 0 },
        { D_80156963, 0 },
        { D_8015697D, 0 }
    };
    Position_800E11C0 position;
    FlagsView_800E11C0 flags;
    char *name;
    u16 sound;
    bool visible;
    s32 blocked;

    blocked = 0;
    if (func_800E1CD4(object, 15)) {
        blocked = 1;
    } else if (object->field_1C & 1) {
        blocked = 1;
    } else if (func_800A6E90(object) && kind != 13) {
        blocked = 1;
    } else {
        s32 in_mode = D_80142F18.mode == 0x4F;
        if (in_mode) {
            blocked = 1;
        }
    }
    if (blocked) {
        return;
    }
    if (object->field_72 & 1) {
        func_800E20F0(object);
    }
    name = func_800A3B20(object);
    visible = (object->field_1E & 0xC) == 0;
    sound = D_80148274[kind][visible];
    flags.bits = object->field_20;
    if (kind == 10 || kind == 11) {
        s32 blocked = 0;
        if (func_800E1CD4(object, 10)) {
            blocked = 1;
        } else if (func_800E1CD4(object, 11)) {
            blocked = 1;
        }
        if (blocked) {
            return;
        }
        if (flags.test(3)) {
            func_800498E4(0x18D, name);
            return;
        }
    } else if (kind == 8) {
        u8 count;
        if (flags.test(11)) {
            func_800498E4(0x224, name);
            return;
        }
        count = func_800E0F40(object);
        if (count < 2) {
            return;
        }
    } else if (kind == 7) {
        if (flags.test(2)) {
            func_800498E4(0x224, name);
            return;
        }
        if (object->field_2C < 2) {
            return;
        }
    }
    if (duration == 0xFE) {
        if (duration_range[kind][1] == 0) {
            duration = duration_range[kind][0];
        } else {
            duration = func_800C5844(D_80147620, duration_range[kind][0], duration_range[kind][1]);
        }
    }
    if (duration != 0xFF) {
        duration++;
    }
    if (kind < 10) {
        if (object->status_turns[kind] != 0) {
            return;
        }
        if (kind == 2 && !(object->field_1E & 0xC)) {
            func_800E03BC(object, 1);
            object->vtbl->react((char *)object + object->vtbl->react_delta, 1, 0x11, 0, 0);
        }
        if (kind == 0 && ((object->field_1E >> 2) & 1)) {
            object->field_E4 &= ~8;
        }
        if (kind == 9) {
            blocked = 0;
            if ((object->field_1E & 0xC) || func_800A44F4(D_801476B8, object) == 1) {
                blocked = 1;
            }
            if (!blocked) {
                func_800E4734(object, 0, 0xA1);
                return;
            }
            func_800E43EC(object, 0xD5, 1);
        }
        object->status_turns[kind] = duration;
    } else if (kind < 17) {
        if (object->field_42 != 0) {
            if (func_800E10D0(object) == kind) {
                return;
            }
            if (kind == 15) {
                func_80049CB4(6);
                func_800E18C0(object, func_800E10D0(object), 0, 0);
                func_80049CB4(7);
            }
        }
        func_800E10A4(object, kind);
        object->field_42 = duration;
        if (kind == 15) {
            position = object->position;
            if ((func_800B1C6C(&position) & 0x2000) && (object->field_09 & 0xF) == 2) {
                func_80049CB4(0x10B, &position);
                func_800A5A70(object, 1);
            }
            func_800E43EC(object, 0xD6, 1);
        } else if (kind == 16) {
            position = object->position;
            object->field_71 = position.y;
            object->field_70 = position.x;
        }
    } else if (kind < 19) {
        u8 current;
        s32 level = (u8)func_800E04D0(object) + adjustment;
        if (level < 0) {
            level = 0;
        } else if (level > 3) {
            level = 3;
        }
        if (object->field_43 != 0) {
            current = func_800E110C(object);
            if (current == level) {
                return;
            }
        }
        func_800E10E0(object, level);
        object->field_50 = D_80158C6C[level];
        object->field_43 = duration;
    } else if (kind < 21) {
        if (object->field_44 != 0 && func_800E1148(object) == kind) {
            return;
        }
        func_800E1118(object, kind);
        object->field_44 = duration;
    } else {
        return;
    }
    func_80049CB4(0xA6, object, kind, 0);
    func_80049CB4(0x1F, object);
    if (sound != 0) {
        func_800498E4(sound, name);
    }
    object->vtbl->refresh((char *)object + object->vtbl->refresh_delta);
    func_800A533C(object);
}

}

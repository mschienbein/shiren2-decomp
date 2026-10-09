#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

/* Bit reader over a packed buffer: buffer pointer, bit position, size (func_800A09B0). */
typedef struct {
    void *buffer;
    u32 position;
    u32 size;
} BitStream;

/* Per-slot entry: converted kind byte and two flag bits. */
typedef struct {
    u8 kind96;
    u8 flags97;
} Slot;

/* Unpacked 0x9A-byte save record (0x90-byte packed form). */
typedef struct {
    u8 unk0, unk1, unk2, unk3;
    u8 name4[4];
    u8 unk8, unk9, unkA, padB;
    u32 unkC;
    u32 unk10;
    u8 unk14, unk15, unk16;
    u8 text17[0x50];
    u8 unk67;
    u16 unk68;
    u8 pad6A[2];
    u32 unk6C;
    u8 unk70, unk71, unk72;
    u8 text73[0x10];
    u8 unk83, unk84, unk85;
    u8 text86[0x10];
    Slot slots[2];
} SaveRecord;

extern void *func_800A09B0(BitStream *stream, void *buffer, u32 size);
extern void func_800A0B74(void *stream, unsigned char *output, u32 bit_count);
extern void func_800A0BB0(void *stream, unsigned char *output, s32 byte_count);
extern void func_800A0ACC(BitStream *stream, u32 *output, u32 bit_count);
extern void func_800A0B38(BitStream *stream, u16 *output, u32 bit_count);
extern s32 func_800CB5F8(u8 first, u8 base);

/* Countdown terminator for the two-slot loop: the slot index running past 0. */
static inline s32 Last(void) { return -1; }

void func_800CB96C(void *packet, SaveRecord *rec) {
    BitStream reader;
    u8 tmp;
    s32 i;
    Slot *slot;

    func_800A09B0(&reader, packet, 0x90);
    func_800A0B74(&reader, &rec->unk0, 5);
    func_800A0B74(&reader, &rec->unk1, 7);
    func_800A0B74(&reader, &rec->unk2, 7);
    func_800A0B74(&reader, &rec->unk3, 7);
    func_800A0BB0(&reader, rec->name4, 4);
    func_800A0B74(&reader, &rec->unk8, 7);
    func_800A0B74(&reader, &rec->unk9, 8);
    func_800A0B74(&reader, &rec->unkA, 7);
    func_800A0ACC(&reader, &rec->unkC, 0x1E);
    func_800A0ACC(&reader, &rec->unk10, 0x1B);
    func_800A0B74(&reader, &rec->unk14, 8);
    func_800A0B74(&reader, &rec->unk15, 7);
    func_800A0B74(&reader, &rec->unk16, 7);
    func_800A0BB0(&reader, rec->text17, 0x50);
    func_800A0B74(&reader, &rec->unk67, 4);
    func_800A0B38(&reader, &rec->unk68, 0xA);
    func_800A0ACC(&reader, &rec->unk6C, 0x18);
    func_800A0B74(&reader, &tmp, 6);
    rec->unk70 = func_800CB5F8(tmp, 0x32);
    func_800A0B74(&reader, &rec->unk71, 8);
    func_800A0B74(&reader, &rec->unk72, 5);
    func_800A0BB0(&reader, rec->text73, 0x10);
    func_800A0B74(&reader, &tmp, 6);
    rec->unk83 = func_800CB5F8(tmp, 0x59);
    func_800A0B74(&reader, &rec->unk84, 8);
    func_800A0B74(&reader, &rec->unk85, 5);
    func_800A0BB0(&reader, rec->text86, 0x10);
    slot = rec->slots;
    /* ODD_C: the end sentinel comes from the Last() inline rather than a literal -1.
     * With a literal, GCC folds the first test (2 - 1 != -1) and rotates the loop,
     * moving the decrement-and-compare to the bottom. Through the inline it keeps the
     * test at the top (addiu s2,-1; beq s2,s4 at 0x800CBB68) with -1 hoisted into s4,
     * as in the ROM. Tried literal -1, unsigned, while, i-- != 0, --i >= 0, upward
     * for, and block-scoped exit flags: all rotate (680-692 of 696 bytes). */
    for (i = 2; --i != Last(); slot++) {
        func_800A0B74(&reader, &tmp, 5);
        slot->kind96 = func_800CB5F8(tmp, 0x7B);
        func_800A0B74(&reader, &tmp, 1);
        if (tmp) slot->flags97 |= 1; else slot->flags97 &= ~1;
        func_800A0B74(&reader, &tmp, 1);
        if (tmp) slot->flags97 |= 2; else slot->flags97 &= ~2;
    }
}

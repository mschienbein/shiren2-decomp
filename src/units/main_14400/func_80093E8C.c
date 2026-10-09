#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct { u8 bytes[0x22]; } Payload;
typedef struct { s16 delta, index; void (*call)(void *, s32, void *); } ReadSlot;
typedef struct { u8 pad00[0x28]; ReadSlot read; } StreamVTable;
typedef struct { s32 position, end; u8 pad08[0x0C]; s32 error; StreamVTable *vtable; } Stream;
typedef struct { Stream *stream; s8 mode04, mode05, mode06, mode07; u8 command08; Payload saved09; Payload pending2B; u8 pad4D[0x13]; u8 repeat60; } Object;
typedef struct { s16 delta, index; s32 (*call)(void *); } CheckSlot;
typedef struct { u8 pad00[0x20]; CheckSlot check; } CommandVTable;
typedef struct { u32 kind; CommandVTable *vtable; } Command;
typedef struct { u8 kind, variant, field02, flags, row, field05, field06, field07, mode; s8 coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
extern const u8 D_80151A3C[];
extern const char D_80151C78[], D_80151CA4[], D_80151CC0[];
extern void func_800C96FC(void);
extern void func_80094B3C(Object *, s32);
extern void *func_800C9E10(void);
extern void func_80045250(void *, s32);
extern void func_800CA20C(Stream *, s32);
extern void func_800CA250(Stream *, s32);
extern void func_80136964(const char *, ...);
extern void *func_800D8FB0(u32);
extern Command *func_800D90E8(void *storage, u8 *data);
extern Command *func_800D92E8(void *storage, u8 *data);
extern Command *func_800D94C8(void *storage, u8 *data);
extern Command *func_800D9784(void *storage, u8 *data);
extern Command *func_800D9928(void *storage, u8 *data);
extern Command *func_800D9C00(void *storage, u8 *data);
extern Command *func_800D9D4C(void *storage, u8 *data);
extern Command *func_800D9DF8(void *storage, u8 *data);
extern Command *func_800D9EB8(void *storage, u8 *data);
extern Command *func_800D9F48(void *storage, u8 *data);
extern Command *func_800DA038(void *storage, u8 *data);
extern Command *func_800DA1C4(void *storage, u8 *data);
extern Command *func_800DA268(void *storage, u8 *data);
extern Command *func_800DA2F8(void *storage, u8 *data);
extern Command *func_800DA388(void *storage, u8 *data);
extern Command *func_800DA418(void *storage, u8 *data);
extern Command *func_800DA4D0(void *storage, u8 *data);
extern Command *func_800DA5AC(void *storage, u8 *data);
extern Command *func_800DA648(void *storage, u8 *data);
extern Command *func_800DAE98(void *storage, u8 *data);
extern Command *func_800DAF68(void *storage, u8 *data);
extern Command *func_800DB0BC(void *storage, u8 *data);
extern Command *func_800DB1BC(void *storage, u8 *data);
extern Command *func_800DB410(void *storage, u8 *data);
extern Command *func_800DB738(void *storage, u8 *data);
extern Command *func_800DB928(void *storage, u8 *data);
extern Command *func_800DBB1C(void *storage, u8 *data);
extern Command *func_800DBC88(void *storage, u8 *data);
extern Command *func_800DBF14(void *storage, u8 *data);
extern Command *func_800DC1CC(void *storage, u8 *data);
extern Command *func_800DC380(void *storage, u8 *data);
extern Command *func_800DC448(void *storage, u8 *data);
extern Command *func_800DC650(void *storage, u8 *data);
extern Command *func_800DC968(void *storage, u8 *data);
extern Command *func_800DCA84(void *storage, u8 *data);
extern Command *func_800DCC00(void *storage, u8 *data);
extern Command *func_800DCDEC(void *storage, u8 *data);
extern Command *func_800DCF64(void *storage, u8 *data);
extern Command *func_800DD0F4(void *storage, u8 *data);
extern Command *func_800DD298(void *storage, u8 *data);
extern Command *func_800DD378(void *storage, u8 *data);
extern Command *func_800DD448(void *storage, u8 *data);
extern Command *func_800DD518(void *storage, u8 *data);
extern Command *func_800DD66C(void *storage, u8 *data);
extern Command *func_800DD7D4(void *storage, u8 *data);
extern Command *func_800DD904(void *storage, u8 *data);
extern Command *func_800DDA64(void *storage, u8 *data);
extern Command *func_800DDE88(void *storage, u8 *data);
extern Command *func_800DE1D8(void *storage, u8 *data);
extern Command *func_800DE538(void *storage, u8 *data);
extern Command *func_800DE7EC(void *storage, u8 *data);
extern Command *func_800DEC68(void *storage, u8 *data);
extern Command *func_800DEEE8(void *storage, u8 *data);
extern Command *func_800DF3C0(void *storage, u8 *data);
extern Command *func_800DF6E0(void *storage);
extern Command *func_800DF8E8(void *storage, u8 *data);
extern Command *func_800DF9C8(void *storage, u8 *data);
extern Command *func_800DFCA8(void *storage, u8 *data);
extern Command *func_800DFEF8(void *storage, u8 *data);
extern Command *func_800E00BC(void *storage, u8 *data);

/* The concrete memory stream binds void func_800CA668(self, count, destination). */
static inline void stream_read(Stream *stream, s32 count, void *data) {
    ReadSlot *slot = &stream->vtable->read;
    slot->call((u8 *)stream + slot->delta, count, data);
}
Command *func_80093E8C(Object *self) {
    static s32 D_801C3470;
    u8 opcode;
    Command *command;
    Command *result;
    s32 bad;
    if (!self->repeat60) {
        if (self->stream->end - self->stream->position <= 0) {
            func_800C96FC();
            if ((D_80142F18.flags >> 2) & 1) func_80094B3C(self, 2);
            else func_80094B3C(self, 0);
            return 0;
        }
        D_801C3470 = self->stream->position;
        stream_read(self->stream, 1, &opcode);
        if (opcode & 0x80) self->repeat60 = (opcode & 0x7F) + 1;
    }
    if (self->repeat60) {
        self->repeat60--;
        opcode = self->command08;
        self->pending2B = self->saved09;
    } else if (opcode < 0x3F) {
        s32 size = D_80151A3C[opcode];
        self->command08 = opcode;
        switch (size) {
        case 0xFF:
            stream_read(self->stream, 1, self->pending2B.bytes);
            stream_read(self->stream, self->pending2B.bytes[0], self->pending2B.bytes + 1);
            break;
        default:
            if (size-- >= 2) {
                stream_read(self->stream, size, self->pending2B.bytes);
            }
            break;
        }
        self->saved09 = self->pending2B;
    }
    switch (opcode) {
    case 0x1:
        command = func_800DF8E8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x2:
        command = func_800DF3C0(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0x3:
        command = func_800DA5AC(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0x4:
        command = func_800DA648(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x5:
        command = func_800D90E8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x6:
        command = func_800D92E8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x7:
        command = func_800D94C8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x8:
        command = func_800D9784(func_800D8FB0(0xC8), self->pending2B.bytes);
        break;
    case 0x9:
        command = func_800D9928(func_800D8FB0(0xC4), self->pending2B.bytes);
        break;
    case 0xA:
        command = func_800D9C00(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0xB:
        command = func_800D9D4C(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0xC:
        command = func_800DAE98(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0xD:
        command = func_800DAF68(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0xE:
        command = func_800DB0BC(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0xF:
        command = func_800DB1BC(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x10:
        command = func_800DB410(func_800D8FB0(0x18), self->pending2B.bytes);
        break;
    case 0x11:
        command = func_800DB738(func_800D8FB0(0x18), self->pending2B.bytes);
        break;
    case 0x12:
        command = func_800DB928(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x13:
        command = func_800DBB1C(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x14:
        command = func_800DBC88(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x15:
        command = func_800DBF14(func_800D8FB0(0x18), self->pending2B.bytes);
        break;
    case 0x16:
        command = func_800DC1CC(func_800D8FB0(0x18), self->pending2B.bytes);
        break;
    case 0x17:
        command = func_800DC380(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x18:
        command = func_800DC448(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x19:
        command = func_800DC650(func_800D8FB0(0x18), self->pending2B.bytes);
        break;
    case 0x1A:
        command = func_800DC968(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x1B:
        command = func_800DCA84(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x1C:
        command = func_800DCC00(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x1D:
        command = func_800DCDEC(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x1E:
        command = func_800DCF64(func_800D8FB0(0x14), self->pending2B.bytes);
        break;
    case 0x1F:
        command = func_800DD298(func_800D8FB0(0x14), self->pending2B.bytes);
        break;
    case 0x20:
        command = func_800DD0F4(func_800D8FB0(0x14), self->pending2B.bytes);
        break;
    case 0x21:
        command = func_800DD378(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x22:
        command = func_800DD448(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x23:
        command = func_800DD518(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x24:
        command = func_800DD66C(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x25:
        command = func_800DD7D4(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x26:
        command = func_800DD904(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x27:
        command = func_800DDA64(func_800D8FB0(0x10), self->pending2B.bytes);
        break;
    case 0x28:
        command = func_800DDE88(func_800D8FB0(0xC4), self->pending2B.bytes);
        break;
    case 0x29:
        command = func_800DE1D8(func_800D8FB0(0xC4), self->pending2B.bytes);
        break;
    case 0x2A:
        command = func_800DE538(func_800D8FB0(0xC8), self->pending2B.bytes);
        break;
    case 0x2B:
        command = func_800DE7EC(func_800D8FB0(0xCC), self->pending2B.bytes);
        break;
    case 0x2C:
        command = func_800DEC68(func_800D8FB0(0xC4), self->pending2B.bytes);
        break;
    case 0x2D:
        command = func_800DEEE8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x2F:
        command = func_800DF9C8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x30:
        command = func_800DFCA8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x31:
        command = func_800D9DF8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x32:
        command = func_800D9EB8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x33:
        command = func_800D9F48(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x34:
        command = func_800DA038(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x35:
        command = func_800DFEF8(func_800D8FB0(8), self->pending2B.bytes);
        break;
    case 0x37:
        command = func_800DA2F8(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0x38:
        command = func_800DA388(func_800D8FB0(0x18), self->pending2B.bytes);
        break;
    case 0x39:
        command = func_800DA418(func_800D8FB0(0x14), self->pending2B.bytes);
        break;
    case 0x3A:
        command = func_800DA4D0(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0x3B:
        command = func_800DA268(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0x3C:
        command = func_800DA1C4(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0x3E:
        command = func_800E00BC(func_800D8FB0(0xC), self->pending2B.bytes);
        break;
    case 0x0:
    case 0x2E:
        command = func_800DF6E0(func_800D8FB0(8));
        break;
    default:
        func_80136964(D_80151C78, opcode);
        command = 0;
        self->repeat60 = 0U;
        break;
    }
    if (!self->repeat60 && self->stream->end - self->stream->position <= 0) {
        func_800C96FC();
        if (self->mode06 == 2 || self->mode06 == 0) func_80045250(func_800C9E10(), 1);
    }
    bad = 0;
    {
        s32 error = self->stream->error;
        if (error) {
            bad = 1;
            func_80136964(D_80151CA4, error);
            self->stream->error = 0;
        }
    }
    if (command) {
        CheckSlot *slot = &command->vtable->check;
        s32 invalid = slot->call((u8 *)command + slot->delta) != 1;
        if (invalid) {
            bad = 1;
            func_80136964(D_80151CC0);
        }
    } else {
        bad = 1;
    }
    if (!bad) {
        result = command;
    } else {
        self->repeat60 = 0;
        func_800CA20C(self->stream, 0);
        func_800CA250(self->stream, D_801C3470);
        func_800C96FC();
        result = 0;
    }
    return result;
}

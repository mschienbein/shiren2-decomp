#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct {
    u8 pad0[4]; u16 status, field6, state, position, fieldC, fieldE;
    u8 pad10[4]; s32 entity; u8 pad18[4]; s32 delay;
    u16 *script; s32 counter, otherSide, side;
    u8 pad30[0x2C]; s32 field5C, field60, field64;
} Task;
typedef struct {
    u8 pad0[0xC]; s16 x, y, z; u8 pad12[0xA];
    float field1C, field20, field24; u8 pad28[0x10];
    float field38; u16 field3C; u8 animation; u8 pad3F[7]; u8 alpha;
} Unit;
extern void func_800840C0(Task *);
extern Unit *func_8007946C(s32, s32);
extern void func_8007935C(s32);
extern void func_80079560(s32, s32, s32);
extern s32 func_80076044(s32, s32, s32, s32, s32);
extern void func_8008B8A8(s32, s32, s32, s32);
extern s32 func_80077BA4(s32, s32);

void func_8008B004(void *task)
{
    Task *self = task;
    switch (self->state) {
    case 0:
        func_800840C0(self);
        self->position = 0;
        self->delay = 0;
        ++self->state;
    case 1:
        {
            Unit *unit;
            s32 finished = 0;
            u16 *cursor;
            u16 opcode, value, mode;
            s16 high;
            unit = func_8007946C(self->side, self->entity);
            if (self->delay && --self->delay) {
                return;
            }
            do {
                s32 step = 2;
                cursor = self->script;
                cursor += self->position;
                opcode = *cursor++;
                switch (opcode) {
                case 0x7F00:
                    value = *cursor;
                    self->fieldE = value;
                    break;
                case 0x7F01:
                    func_8007935C(self->entity);
                    break;
                case 0x7F02:
                    func_80079560(self->side, self->entity, (s16)*cursor);
                    break;
                case 0x7F03:
                    func_80079560(self->otherSide, self->counter, (s16)*cursor);
                    break;
                case 0x7F04:
                    self->delay = (s16)*cursor;
                    ++finished;
                    break;
                case 0x7F05:
                    func_80076044(self->entity, unit->field3C, 2, (s16)*cursor, 0);
                    break;
                case 0x7F06:
                    func_80076044(self->entity, unit->field3C, 1, (s16)*cursor, 3);
                    break;
                case 0x7F0F:
                    unit->field38 = (s16)*cursor / 100.0f;
                    break;
                case 0x7F10:
                    value = *cursor;
                    unit->alpha = value;
                    break;
                case 0x7F11:
                    unit->alpha -= ((u8 *)cursor)[1];
                    break;
                case 0x7F07:
                    value = *cursor++;
                    high = ((s16)cursor[1] & 0xFF00) >> 8;
                    mode = *cursor++;
                    func_80076044(self->entity, (s16)value, high, (s16)mode,
                        ((u8 *)cursor)[1]);
                    self->position += 2;
                    break;
                case 0x7F0E:
                    func_8008B8A8(self->entity, self->field5C, self->field60, self->field64);
                    break;
                case 0x7F0A:
                    unit->x = *cursor++;
                    unit->y = cursor[0];
                    unit->z = cursor[1];
                    self->position += 2;
                    break;
                case 0x7F0C:
                    unit->x = unit->x + (s16)*cursor * 4.0f / 10.0f;
                    break;
                case 0x7F0D:
                    unit->x = *cursor;
                    break;
                case 0x7F0B:
                    unit->field1C = (s16)*cursor++ / 100.0f;
                    unit->field20 = (s16)cursor[0] / 100.0f;
                    unit->field24 = (s16)cursor[1] / 100.0f;
                    self->position += 2;
                    break;
                case 0x7F08:
                    func_80077BA4((s16)*cursor, -1);
                    break;
                case 0x7F09:
                    func_80077BA4((s16)*cursor, 0);
                    break;
                case 0x7F13:
                    self->counter = (s16)*cursor;
                    break;
                case 0x7F14:
                    value = *cursor;
                    if (--self->counter) {
                        self->position = (s16)value * 2;
                        step = 0;
                    }
                    break;
                case 0x7FFF:
                    ++finished;
                    self->status = 4;
                    break;
                default:
                    if (!self->delay) {
                        value = *cursor;
                        unit->animation = opcode;
                        self->delay = (s16)value;
                    }
                    ++finished;
                    break;
                case 0x7F12:
                    /* ODD_C: the script format includes an explicit no-op command. */
                    break;
                }
                self->position += step;
            } while (!finished);
        }
        break;
    }
}

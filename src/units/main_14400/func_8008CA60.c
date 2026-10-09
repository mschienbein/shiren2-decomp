#include "common.h"
typedef unsigned char u8;
typedef struct { s32 key; void *value; } Entry;
typedef struct { u32 capacity, count, head; Entry *entries; } Queue;
typedef Queue Obj8008D400;
typedef struct { u32 index, field_4; } FileRef;
typedef struct { u8 pad_0[0xC]; u32 count; FileRef *files; } Record;
typedef struct { u8 pad_0[3]; u8 selected, status; u8 pad_5[3]; void **files; u8 pad_C[8]; Record *records; } Resource;
typedef struct State State;
typedef struct Obj Obj;
typedef struct Packet Packet;
struct Packet { u8 pad_0[0xC]; s32 (*execute)(Packet *, Obj *, State *); };
struct State {
    u8 kind, flags; u8 pad_2[2]; u32 field_4; u8 pad_8[4]; u32 field_C, field_10;
    u8 pad_14[7]; u8 field_1B; u8 pad_1C[8]; u8 field_24, field_25, field_26, field_27, field_28, field_29, field_2A, field_2B;
    u8 pad_2C[0xD]; u8 field_39; u8 pad_3A[0x102]; float matrix[16]; u8 pad_17C[0x2C]; float field_1A8; u8 field_1AC; u8 pad_1AD[3]; void *field_1B0; Queue *field_1B4;
};
struct Obj {
    u8 pad_0[2]; u8 flags, field_3, field_4, field_5; u8 pad_6[2]; void *field_8; float field_C, matrix[16]; State *states[9]; Resource *resource;
};
extern s32 D_8013FEF0[];
extern u32 D_8013FF18[];
extern u8 D_801E4E48[];
extern void func_8008D3F0(void *state);
extern void func_80091544(void *ptr);
extern void func_8008D400(Obj8008D400 *queue);
extern s32 func_8008D40C(Queue *queue, u32 capacity);
extern void *func_80091450(u32 size);
extern void func_8008D3D0(void *state);
extern s32 func_8008D4B8(Queue *queue, void *value, s32 key);
extern Packet *func_8008D5A8(Queue *queue);
extern s32 func_80070598(void *manager, void *state);
extern void func_8008D47C(Queue *queue);
s32 func_8008CA60(Obj *obj) {
    Queue queue;
    s32 result = 0;
    u32 count, index;
    if (!(obj->flags & 1)) goto done;
    count = obj->resource->records[obj->resource->selected].count;
    for (index = result; index < 9; index++) {
        if (obj->states[index]) {
            func_8008D3F0(obj->states[index]);
            func_80091544(obj->states[index]);
            obj->states[index] = 0;
        }
    }
    func_8008D400(&queue);
    if (func_8008D40C(&queue, 31)) { result = -1; goto done; }
    else {
        for (index = 0; index < count; index++) {
            State *state;
            State *allocation;
            void *owner;
            Resource *resource;
            Packet *packet;
            s32 i;
            result = 0;
            allocation = func_80091450(0x1B8);
            obj->states[index] = allocation;
            if (!allocation) { result = -1; break; }
            state = allocation;
            func_8008D3D0(state);
            state->field_1A8 = obj->field_C;
            state->field_1AC = obj->field_3;
            owner = obj->field_8;
            state->field_1B4 = &queue;
            state->field_1B0 = owner;
            state->matrix[0] = obj->matrix[0];
            state->matrix[1] = obj->matrix[1];
            state->matrix[2] = obj->matrix[2];
            state->matrix[3] = obj->matrix[3];
            state->matrix[4] = obj->matrix[4];
            state->matrix[5] = obj->matrix[5];
            state->matrix[6] = obj->matrix[6];
            state->matrix[7] = obj->matrix[7];
            state->matrix[8] = obj->matrix[8];
            state->matrix[9] = obj->matrix[9];
            state->matrix[10] = obj->matrix[10];
            state->matrix[11] = obj->matrix[11];
            state->matrix[12] = obj->matrix[12];
            state->matrix[13] = obj->matrix[13];
            state->matrix[14] = obj->matrix[14];
            state->matrix[15] = obj->matrix[15];
            resource = obj->resource;
            if (func_8008D4B8(&queue, resource->files[resource->records[resource->selected].files[index].index], result)) { result = -1; break; }
            while ((packet = func_8008D5A8(&queue)) != 0) {
                result = packet->execute(packet, obj, state);
                if (result) break;
            }
            if (result) break;
            if (state->flags & 1) {
                if (D_8013FEF0[0] != -1) {
                    for (i = 0; D_8013FEF0[i] != -1; i++) {
                        if (state->field_10 == D_8013FEF0[i]) break;
                    }
                    if (D_8013FEF0[i] != -1) {
                        if (obj->field_5 != 0xFF && state->field_4 == 0) {
                            u32 replacement;
                            state->field_4 = 0x100000;
                            state->field_C = 0x0C080000;
                            replacement = D_8013FF18[i];
                            state->field_24 = 31;
                            state->field_25 = 31;
                            state->field_26 = 31;
                            state->field_27 = 0;
                            state->field_28 = 0;
                            state->field_29 = 7;
                            state->field_2A = 5;
                            state->field_2B = 7;
                            state->field_10 = replacement;
                            state->field_1B = obj->field_5;
                        }
                        if (obj->field_4 && state->kind == 1) state->field_39 = obj->field_4;
                    }
                }
                if (obj->flags & 2) {
                    state->field_C &= ~0x10;
                    state->field_10 &= ~0x10;
                }
                func_80070598(D_801E4E48, state);
                if (state->kind == 0) obj->resource->status = 2;
            }
        }
    }
    if (!result) func_8008D47C(&queue);
    done:
    if (result) func_8008D47C(&queue);
    return result;
}

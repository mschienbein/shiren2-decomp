#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
/* A loaded descriptor's tables hold encoded ROM addresses until relocated to RAM buffers
 * (same convention as func_80071CCC's FrameData). */
typedef union { u32 segmented; void *resident; } TableRef;
typedef struct { u8 count_00; u8 pad_01[3]; TableRef table_04; u8 count_08; u8 pad_09[3]; TableRef table_0C; } Descriptor;
typedef struct {
    u16 field_00; short kind_02; u8 field_04, field_05, field_06, field_07, field_08, field_09;
    u16 field_0A, field_0C, field_0E, field_10; u8 field_12, field_13;
    u32 field_14, random_18; float scale_1C, scale_20, scale_24;
    s32 field_28, field_2C, field_30; u8 field_34, field_35, field_36, field_37;
    float field_38; u16 index_3C; u8 field_3E, field_3F, field_40, field_41, field_42, field_43, field_44, field_45, field_46, field_47, field_48, field_49;
    u16 field_4A; Descriptor *descriptor_4C, *buffer_50; void *buffer_54, *buffer_58;
    u8 pad_5C[0x14]; u8 field_70, field_71, field_72, field_73, field_74, field_75, field_76, field_77; u8 pad_78[0x38];
} Actor;
extern s32 D_8013D8CC;
extern Descriptor *D_801D25A0[];
extern u8 D_5000000[], D_00E53DB0[];
extern u32 func_8002A9B0(void);
extern void func_8006AAF0(void *, u32, s32);
static inline s32 range_end(s32 index, s32 count) { return index + count; }
s32 func_80074500(Actor *actors, s32 index, s32 count, s32 kind, s32 resource) {
    Actor *actor;
    Descriptor *descriptor;
    if (!D_8013D8CC) return -1;
    if (count) {
        count = range_end(index, count);
        for (; index < count; index++) if (actors[index].kind_02 == -1) break;
        if (index >= count) return -1;
    }
    actor = &actors[index];
    actor->field_00 = 0;
    actor->kind_02 = kind;
    actor->field_04 = 0; actor->field_05 = 0; actor->field_06 = 0; actor->field_07 = 0; actor->field_08 = 0; actor->field_09 = 0;
    actor->field_0A = 0; actor->field_0C = 0; actor->field_0E = 0; actor->field_10 = 0;
    actor->field_14 = 0;
    actor->random_18 = func_8002A9B0() & 0x1F;
    actor->field_4A = 0;
    actor->scale_1C = 1.0f; actor->scale_20 = 1.0f; actor->scale_24 = 1.0f;
    actor->field_28 = 0; actor->field_2C = 0; actor->field_30 = 0;
    actor->field_34 = 0; actor->field_35 = 0; actor->field_36 = 3; actor->field_37 = 0;
    actor->field_12 = 0; actor->field_13 = 0; actor->field_38 = 0;
    actor->index_3C = resource;
    actor->field_3E = 0; actor->field_3F = 0; actor->field_40 = 0; actor->field_42 = 0; actor->field_43 = 0; actor->field_41 = 0; actor->field_44 = 0; actor->field_45 = 0;
    actor->field_70 = 0xFF; actor->field_71 = 0xFF; actor->field_72 = 0xFF;
    actor->field_73 = 0; actor->field_74 = 0; actor->field_75 = 0; actor->field_76 = 0; actor->field_77 = 0;
    actor->field_46 = 0xFF; actor->field_47 = 0xFF; actor->field_48 = 0; actor->field_49 = 0;
    descriptor = D_801D25A0[resource];
    if (descriptor) actor->descriptor_4C = descriptor;
    else {
        if (!actor->buffer_50 || !actor->buffer_58 || !actor->buffer_54) return -1;
        /* The DMA API takes encoded ROM device addresses, not dereferenceable CPU pointers. */
        func_8006AAF0(actor->buffer_50, (u32)D_00E53DB0 + ((u32)D_5000000 & 0xFFFFFF) + resource * 16, 0x10);
        func_8006AAF0(actor->buffer_58, (u32)D_00E53DB0 + (actor->buffer_50->table_0C.segmented & 0xFFFFFF), actor->buffer_50->count_08 * 12);
        func_8006AAF0(actor->buffer_54, (u32)D_00E53DB0 + (actor->buffer_50->table_04.segmented & 0xFFFFFF), (actor->buffer_50->count_00 & 0x1F) * 4);
        actor->buffer_50->table_0C.resident = actor->buffer_58;
        actor->buffer_50->table_04.resident = actor->buffer_54;
        actor->descriptor_4C = actor->buffer_50;
    }
    return index;
}

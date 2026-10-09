#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Pos;
typedef struct Dir { u8 v; } Dir;
typedef struct { char pad0[8]; short adjustment; short padA; s32 (*event)(void *, s32, Pos *, Dir); } TraceTable;
typedef struct { Pos pos; Dir direction; s32 distance, limit, mode; u8 changed; TraceTable *table; } Trace;
typedef struct { char pad0[0x10]; short adjustment; short pad12; s32 (*test)(void *); } EntityTable;
typedef struct { char pad0[9]; u8 flags9; char padA[0x12]; u16 flags1C; char pad1E[6]; EntityTable *table; } Entity;
typedef struct { char pad0[3]; u8 type; } Ground;
void func_800A2758(Pos *p, Dir d);
void *func_800B4928(Pos *pos);
void *func_800B4D80(Pos *pos);
s32 func_800B1AB8(Pos *pos);
s32 func_800B1DF8(Pos *pos);
u32 func_800B1C6C(Pos *pos);
void *func_800B31E8(void *pos, s32 team);
s32 func_800B56F0(void *object);
s32 func_800A58B8(Entity *entity);
s32 func_800B58E4(s32 mode, s32 arg, s32 info, s32 value);
static inline Pos *copy_pos(Pos *out, Pos *from) { out->x = from->x; out->y = from->y; return out; }
/* Trace event slot (+0xC, this-delta at +8). Concrete targets 800C37B8,
   800C4468 and 80112084 share one contract: s32 event code (switch bounds use
   sltiu, so the code's signedness is not observable; the canonical direct
   callers of func_80112084 pass s32), and a one-byte Dir aggregate. */
static inline s32 send_event(Trace *trace, s32 event, Pos *position) {
    Pos copy;
    copy.x = position->x;
    copy.y = position->y;
    return trace->table->event((char *)trace + trace->table->adjustment, event, &copy, trace->direction);
}
/* ODD_C: the wall-flag and team queries are position accessors; their
   inlined parameter keeps &position in a register that the following event
   copy reuses (the original re-materializes sp+16 into s0 for these two). */
static inline u32 cell_flags(Pos *p) { return func_800B1C6C(p); }
static inline void *team_at(Pos *p, s32 team) { return func_800B31E8(p, team); }
s32 func_800C2D3C(Trace *trace) {
    s32 proceed = 1, handled, blocked, hit;
    Pos position;
    Entity *entity;
    Ground *ground;
    copy_pos(&position, &trace->pos);
    func_800A2758(&trace->pos, trace->direction);
    handled = 0;
    blocked = 0;
    trace->changed = 0;
    ++trace->distance;
    entity = func_800B4928(&position);
    ground = func_800B4D80(&position);
    if (!func_800B1AB8(&position) || func_800B1DF8(&position)) blocked = 1;
    if (blocked) {
        proceed = send_event(trace, 0, &position);
        if (trace->changed) return proceed;
        handled = 1;
    }
    if (proceed) {
        if (cell_flags(&position) & 0x4000) {
            proceed = send_event(trace, 1, &position);
            if (trace->changed) return proceed;
            handled = 1;
        }
    }
    if (proceed) {
        if (team_at(&position, 0xA)) {
            proceed = send_event(trace, 2, &position);
            if (trace->changed) return proceed;
            handled = 1;
        }
    }
    if (proceed && func_800B56F0(&position)) {
        proceed = send_event(trace, 3, &position);
        if (trace->changed) return proceed;
        if (!proceed) handled = 1;
    }
    hit = 0;
    if (proceed && entity && !(entity->flags1C & 1) &&
        !entity->table->test((char *)entity + entity->table->adjustment)) {
        s32 flags = entity->flags9 & 0xF;
        hit = func_800B58E4(trace->mode, trace->mode, flags, func_800A58B8(entity)) != 0;
    }
    if (hit) {
        proceed = send_event(trace, 4, &position);
        if (trace->changed) return proceed;
        handled = 1;
    }
    if (proceed) {
        if (ground && func_800B58E4(trace->mode, trace->mode, ground->type, ground->type)) {
            proceed = send_event(trace, 5, &position);
            if (trace->changed) return proceed;
            handled = 1;
        }
        if (proceed) {
            if (trace->distance >= trace->limit) {
                proceed = send_event(trace, 6, &position);
                if (trace->changed) return proceed;
                handled = 1;
            }
            if (proceed && !handled) proceed = send_event(trace, 7, &position);
        }
    }
    return proceed;
}

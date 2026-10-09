#include "common.h"

/*
 * Party follower placement after the leader moves (g++ 2.8.1 TU).
 * C++ evidence: the non-independent branch begins with the empty count-down
 * loop g++ emits when it constructs `Point occupied[4]` (empty default
 * constructor), and that array reuses the stack slots of the independent
 * branch's block-scoped points (g++ frees and combines block slots; 0x28..0x47).
 * Point copies made by its member-wise copy constructor load/store field by
 * field, while plain assignments use the implicit bitwise operator= (block
 * moves); the ring centre is passed by value to an inline helper.
 */

typedef unsigned char u8;
typedef unsigned short u16;

struct Point {
    s32 x;
    s32 y;
    Point() {}
    Point(s32 px, s32 py) : x(px), y(py) {}
    Point(const Point &other) : x(other.x), y(other.y) {}
    void set(s32 px, s32 py) { x = px; y = py; }
    s32 isSet() { return y | x; }
};

struct Rect {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
};

/* 0x14-byte room record: bounding rectangle plus four counts (func_800B68B0 sums them). */
struct Room {
    Rect bounds;
    u8 counts[4];
};

/* 0x18-byte room-group record (cleared by func_800D1D90); +4 is its room. */
struct Region {
    u8 field_00;
    u8 pad01[3];
    Room *room;
    u8 field_08;
    u8 pad09[3];
    s32 field_0C;
    s32 field_10;
    s32 field_14;
};

/* Leading part of an entity record returned by func_800A910C. */
struct Entity {
    u8 pad00[0x80];
    Region *region;
    Point field_84;
    Point field_8C;
};

/* Entity iterator (func_800A9070 advances to the next entity of a kind). */
struct EntityIterator {
    s32 index;
    EntityIterator() : index(0) {}
    Entity *next();
};

/* 0x18-byte ring iterator built by func_800C5280. */
struct PointIterator {
    u8 bytes[0x18];
};

struct SelectionRecord {
    unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode;
    signed char coordinates[2], status;
};

extern "C" {
extern SelectionRecord D_80142F18;
extern u8 D_80143391;
extern Point D_80147F70;
extern Rect D_801429C0;
void *func_800B3774(Point *out);
s32 func_800A251C(Point *a, Point *b);
s32 func_800A24DC(Point *point, Rect *bounds);
s32 func_800A9070(EntityIterator *iterator, s32 kind);
Entity *func_800A910C(EntityIterator *iterator);
s32 func_800A31C8(Room *room, Point *point);
s32 func_800D1E90(Region *region, Entity *entity, Point *start, Point *from, Point *to, s32 flags);
void func_800F61B4(Entity *entity, Point *from, Point *to);
s32 func_800B68B0(Room *room);
void *func_800B6A98(Point *out, Room *room, s32 index);
void func_800F61D8(Entity *entity);
void func_800F61FC(Entity *entity);
s32 func_800A41EC(Entity *entity, Point *point);
void *func_800C5280(PointIterator *ring, Point *center, unsigned short kind);
s32 func_800C559C(PointIterator *ring);
Point *func_800C532C(Point *out, PointIterator *ring);
}

/* func_800A251C reports equality as 0/1; this is its negation. */
static inline s32 differs(Point *a, Point *b)
{
    return func_800A251C(a, b) ^ 1;
}

static inline void release(Entity *entity)
{
    if (D_80143391 & 4) {
        func_800F61D8(entity);
    } else {
        func_800F61FC(entity);
    }
}

static inline s32 floor_flag(s32 mask)
{
    return D_80143391 & mask;
}

inline Entity *EntityIterator::next()
{
    return func_800A910C(this);
}

static inline void ring_begin(PointIterator *ring, Point center, u16 kind)
{
    func_800C5280(ring, &center, kind);
}

extern "C" void func_800D37E8(s32 reset)
{
    s32 invalid = (D_80142F18.mode & 0xE0) != 0x20;
    if (invalid) {
        return;
    }
    if (reset) {
        D_80147F70.set(0, 0);
    }
    Point previous(D_80147F70);
    Point current;
    func_800B3774(&current);
    s32 moved = differs(&current, &D_80147F70);
    D_80147F70 = current;
    EntityIterator entities;
    s32 independent = !floor_flag(4) && !floor_flag(8);
    if (independent) {
        s32 sample = 0;
        if (!moved) {
            return;
        }
        entities.index = 0;
        for (;;) {
            s32 active = func_800A9070(&entities, 0x57);
            if (!active) {
                break;
            }
            Entity *entity = entities.next();
            Region *region = entity->region;
            Room *room = region->room;
            if (room == 0) {
                continue;
            }
            if (func_800A31C8(room, &current)) {
                Point from;
                Point to;
                Point start(0, 0);
                if (func_800D1E90(region, entity, &start, &from, &to, 0)) {
                    func_800F61B4(entity, &from, &to);
                }
            } else {
                Point home(entity->field_8C);
                if (func_800A251C(&home, &previous)) {
                    Point step;
                    Point from;
                    Point to;
                    s32 count = func_800B68B0(room);
                    do {
                        func_800B6A98(&step, room, sample);
                        sample++;
                        if (func_800D1E90(region, entity, &step, &from, &to, 0)) {
                            func_800F61B4(entity, &from, &to);
                            break;
                        }
                    } while (sample != count);
                }
            }
        }
    } else {
        Point occupied[4];
        s32 i;
        for (i = 0; i < 4; i++) {
            occupied[i].set(0, 0);
        }
        entities.index = 0;
        s32 count = 0;
        for (;;) {
            s32 active = func_800A9070(&entities, 0x57);
            if (!active) {
                break;
            }
            Entity *entity = entities.next();
            if (moved) {
                release(entity);
            } else {
                Point here(entity->field_84);
                if (func_800A41EC(entity, &here)) {
                    occupied[count] = here;
                    count++;
                }
            }
        }
        /* Like the original, the scan relies on a cleared slot ending the list. */
        s32 index;
        for (index = 0; occupied[index].isSet(); index++) {
        }
        entities.index = 0;
        for (;;) {
            PointIterator ring;
            s32 active = func_800A9070(&entities, 0x57);
            if (!active) {
                break;
            }
            Entity *entity = entities.next();
            if (!moved) {
                Point here(entity->field_84);
                if (func_800A41EC(entity, &here)) {
                    continue;
                }
            }
            ring_begin(&ring, current, 0x37);
            while (func_800C559C(&ring)) {
                Point candidate;
                func_800C532C(&candidate, &ring);
                s32 valid = func_800A24DC(&candidate, &D_801429C0)
                    && func_800A41EC(entity, &candidate)
                    && differs(&candidate, &current)
                    && differs(&candidate, &occupied[0])
                    && differs(&candidate, &occupied[1])
                    && differs(&candidate, &occupied[2])
                    && differs(&candidate, &occupied[3]);
                if (valid) {
                    func_800F61B4(entity, &candidate, &current);
                    occupied[index] = candidate;
                    index++;
                    break;
                }
            }
        }
    }
}

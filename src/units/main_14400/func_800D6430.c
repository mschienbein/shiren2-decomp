#include "common.h"
#include "map_rect.h"

typedef unsigned char u8;
typedef struct { u8 value; } Dir;
extern MapPos D_80148000;
/* func_800D5BF0 stores one direction byte here at VRAM PC 0x800D5C30. */
extern Dir D_80148008;
/* The rectangle contains two actual position subobjects. */
extern MapRect D_8014800C;
extern u8 D_8014802C;
extern const s32 D_80154868[3];
extern u32 func_800B1C6C(MapPos *position);
extern s32 func_800B4FF0(void *position, s32 kind);
extern void *func_800A2594(MapPos *out, void *position, Dir direction);
extern s32 func_800D6AF8(MapPos *from, unsigned char *direction, MapPos *position);
extern s32 func_800D6D04(void *map, MapPos *position, MapPos *best);
extern s32 func_800A251C(MapPos *first, MapPos *second);

/* ODD_C: taking the aggregate's address keeps the zero-valued ternary arm
   in its own byte temporary; the word-valued setter narrows at the store,
   rather than adding the copies produced by a byte-valued setter. */
static inline void set_direction(Dir *out, s32 value)
{
    out->value = value;
}

/* ODD_C: direction values use aggregate temporaries, matching the original ternary shape. */
static inline Dir direction(u8 value)
{
    Dir result;
    set_direction(&result, value);
    return result;
}

/* ODD_C: the byte accessor takes the aggregate's address, like the setter;
   callers retain Dir temporaries rather than a wider scalar direction view. */
static inline u8 direction_value(Dir *direction)
{
    return direction->value;
}

static inline MapPos *position_init(MapPos *out, s32 x, s32 y)
{
    out->x = x;
    out->y = y;
    return out;
}

s32 func_800D6430(MapPos *best, u8 *flag)
{
    MapPos position;
    MapPos candidate;
    MapPos rotated;
    MapPos *pos;
    s32 eligible;
    best->x = 0;
    best->y = 0;
    pos = position_init(&position, D_8014800C.min.x, D_8014800C.min.y);
    /* Every rectangle read names an actual MapRect member; no pointer walks
       between its subobjects. */
    do {
        eligible = 0;
        if ((func_800B1C6C(pos) & 0x800) || func_800B4FF0(pos, 15))
            eligible = 1;
        if (eligible) {
            Dir initial = position.y == D_8014800C.min.y
                ? (position.x == D_8014800C.min.x ? direction(7)
                    : position.x == D_8014800C.max.x ? direction(1) : direction(0))
                : position.x == D_8014800C.min.x
                    ? (position.y == D_8014800C.max.y ? direction(5) : direction(6))
                    : position.y == D_8014800C.max.y
                        ? (position.x == D_8014800C.max.x ? direction(3) : direction(4))
                        : direction(2);
            Dir current;
            func_800A2594(&candidate, &position, initial);
            current = initial;
            if (func_800D6AF8(&D_80148000, &D_80148008.value, &candidate)) {
                s32 better = 0;
                if ((best->y | best->x) == 0 || func_800D6D04(&D_80148000, &candidate, best))
                    better = 1;
                if (better) {
                    s32 count = 3;
                    /* ODD_C: three iterations each consume a fresh decrement
                       result at the loop head, keeping the original top-of-loop
                       guard instead of a rotated loop. */
                    for (;;) {
                        s32 remaining = --count;
                        if (remaining == -1) break;
                        if (func_800B1C6C(&candidate) & 0xE100) {
                            /* The turn offset is a full word added to the byte
                               direction before masking. */
                            s32 turn = D_80154868[count];
                            s32 next = (direction_value(&initial) + turn) & 7;
                            current.value = next;
                            func_800A2594(&rotated, &position, current);
                            candidate = rotated;
                        } else {
                            *best = candidate;
                            D_8014802C = (direction_value(&current) + 4) & 7;
                            break;
                        }
                    }
                }
            }
        }
        if (position.y < D_8014800C.max.y && position.x == D_8014800C.min.x)
            ++position.y;
        else if (position.y == D_8014800C.max.y && position.x < D_8014800C.max.x)
            ++position.x;
        else if (position.y > D_8014800C.min.y && position.x == D_8014800C.max.x)
            --position.y;
        else
            --position.x;
    } while ((func_800A251C(&D_8014800C.min, &position) ^ 1) != 0);
    if ((best->y | best->x) == 0) return 0;
    *flag = 1;
    return 1;
}

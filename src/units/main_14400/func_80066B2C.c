#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x0, y0, x1, y1; u8 pad_10[0x10]; } Room;
typedef struct { s32 width, height; u16 *tiles; s32 room_count; Room *rooms; } MapInfo;
typedef struct { s32 x, y; } Position;
typedef struct { u16 id; u8 flags; u8 pad3; } GroupMark;
typedef struct { s32 field_00; s32 position_count; Position *positions; s32 mark_count; GroupMark *marks; } Group;
typedef struct { s32 count; Group *groups; } GroupList;
/* Map settings block inside the relocated map section. Bytes 6..21 are indexed by a
 * terrain class: func_800669FC's only caller (func_80062C64) passes
 * func_800B200C's result, which is -1 or (flags & 0xF), so 16 entries cover it. */
typedef struct {
    u8 field_00;
    u8 bordered;
    u8 field_02;
    u8 field_03;
    u8 field_04;
    u8 field_05;
    u8 class_values_06[16];
} Settings;
typedef struct { u16 id; u8 pad2[2]; s32 x, y, cols, rows; u16 *tiles; } Patch;
typedef struct { s32 count; Patch *patches; } PatchList;
struct MenuState { MapInfo *map; GroupList *groups; PatchList *patches; void *field_0C; Settings *settings; };
typedef struct MenuState MenuState;
/* 0x34-byte map entry (D_8013C084[]). */
typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    float field_0C, field_10, field_14;
    u32 data_18; /* PI ROM offset, not a CPU pointer. */
    u8 pad_1C[0x14];
    u16 *marks_30;
} Entry;
typedef struct { u16 id; u8 flag; u8 pad3; } MarkRule;

extern MenuState *D_8013B980;
extern Entry *D_8013B984;
s32 D_8013B98C[5] = { 0, 1, 3, 2, 4 };
extern MarkRule D_8013C3C4[];
extern u32 D_8016DC00;
extern s32 D_8016DC04;
extern s32 D_8013B800;
s32 func_800627C4(void);
void func_80067380(void);
void func_800673D8(u16 id, u16 mask, s32 enable, u16 *tilemap);
u8 func_800424EC(s32 id);
unsigned char func_80041EF0(s32 flag);

void func_80066B2C(u16 *canvas, s32 selected) {
    s32 special = 0;
    MapInfo *map = D_8013B980->map;
    GroupList *groups = D_8013B980->groups;
    Settings *settings = D_8013B980->settings;
    s32 mask = 0xFFFF;
    s32 row, column, group_index, item_index;
    u16 *cursor, *source;
    u16 id;
    Entry *overlay;

    if (func_800627C4() == 1 || func_800627C4() == 2) {
        if (func_800627C4() == 1 && D_8016DC00 - 0x10 < 2) {
            special = 1;
            mask = 0xFFF;
        }
        func_80067380();
        if (canvas) {
            u16 *end = canvas + (map->width + 20) * (map->height + 20);
            for (cursor = canvas; cursor < end; cursor++) *cursor &= 0x400;
            if (settings->bordered) {
                for (row = 0; row < 5; row++) {
                    cursor = canvas + row * (map->width + 20);
                    for (column = 0; column < map->width + 20;) { column++; *cursor++ = 0xC020; }
                }
                for (row = 5; row < map->height + 15; row++) {
                    cursor = canvas + row * (map->width + 20);
                    for (column = 4; column >= 0; column--) *cursor++ = 0xC020;
                    column = map->width + 15;
                    cursor = canvas + (row * (map->width + 20) + column);
                    for (; column < map->width + 20;) { column++; *cursor++ = 0xC020; }
                }
                for (row = map->height + 15; row < map->height + 20; row++) {
                    cursor = canvas + row * (map->width + 20);
                    for (column = 0; column < map->width + 20;) { column++; *cursor++ = 0xC020; }
                }
                for (row = 5; row < 10; row++) {
                    column = 5;
                    cursor = canvas + (row * (map->width + 20) + column);
                    for (; column < map->width + 15;) { column++; *cursor++ = 0x8200; }
                }
                for (row = 10; row < map->height + 10; row++) {
                    column = 5;
                    cursor = canvas + (row * (map->width + 20) + column);
                    for (; column < 10; column++) *cursor++ = 0x8200;
                    column = map->width + 10;
                    cursor = canvas + (row * (map->width + 20) + column);
                    for (; column < map->width + 15;) { column++; *cursor++ = 0x8200; }
                }
                for (row = map->height + 10; row < map->height + 15; row++) {
                    column = 5;
                    cursor = canvas + (row * (map->width + 20) + column);
                    for (; column < map->width + 15;) { column++; *cursor++ = 0x8200; }
                }
            } else {
                for (row = 0; row < 10; row++) {
                    cursor = canvas + row * (map->width + 20);
                    for (column = 0; column < map->width + 20;) { column++; *cursor++ = 0xC020; }
                }
                for (row = 10; row < map->height + 10; row++) {
                    cursor = canvas + row * (map->width + 20);
                    for (column = 9; column >= 0; column--) *cursor++ = 0xC020;
                    column = map->width + 10;
                    cursor = canvas + (row * (map->width + 20) + column);
                    for (; column < map->width + 20;) { column++; *cursor++ = 0xC020; }
                }
                for (row = map->height + 10; row < map->height + 20; row++) {
                    cursor = canvas + row * (map->width + 20);
                    for (column = 0; column < map->width + 20;) { column++; *cursor++ = 0xC020; }
                }
            }
            source = map->tiles;
            for (row = 0; row < map->height; row++) {
                cursor = canvas + ((row + 10) * (map->width + 20) + 10);
                for (column = 0; column < map->width;) { column++; *cursor++ |= *source++; }
            }
        }
        D_8016DC04 = -1;
        if (selected >= 0) {
            if (selected >= groups->count) selected = 0;
            D_8016DC04 = selected;
            for (group_index = 0; group_index < groups->count; group_index++) {
                for (item_index = 0; item_index < groups->groups[group_index].mark_count; item_index++) {
                    s32 flags;
                    id = groups->groups[group_index].marks[item_index].id;
                    flags = groups->groups[group_index].marks[item_index].flags;
                    if (group_index != selected) flags = !flags;
                    func_800673D8(id, 0xFFFF, flags, canvas);
                }
            }
        }
        if (special) {
            for (item_index = 0; (id = D_8013C3C4[item_index].id) != 0; item_index++) {
                func_800673D8(id, 0xFFF,
                    D_8013C3C4[item_index].flag & (1 << func_800424EC(D_8013B98C[(id >> 8) - 1])), canvas);
            }
        }
        overlay = D_8013B984;
        if (overlay->marks_30) {
            item_index = 1;
            id = overlay->marks_30[0];
            while (id) {
                func_800673D8(id, mask, func_80041EF0(overlay->marks_30[item_index++]), canvas);
                overlay = D_8013B984;
                id = overlay->marks_30[item_index++];
            }
        }
        if (canvas) {
            for (group_index = 0; group_index < groups->count; group_index++) {
                if (selected < 0 || selected == group_index) {
                    for (item_index = 0; item_index < groups->groups[group_index].position_count; item_index++) {
                        canvas[groups->groups[group_index].positions[item_index].y * (map->width + 20)
                               + groups->groups[group_index].positions[item_index].x] &= 0x1E5F;
                    }
                }
            }
        }
        if (func_800627C4() == 2) D_8013B800 = 1;
    }
}

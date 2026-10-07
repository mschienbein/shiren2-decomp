#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad[0x20]; s32 type; u8 active; u8 flags; u8 pad26[2]; } Obj;
typedef struct { u16 category; u8 pad[0x2A]; } ObjType;
typedef struct { ObjType *types; } ObjTypeTable;
typedef struct { u16 id; u8 pad2[2]; s32 x; s32 y; s32 cols; s32 rows; u16 *tiles; } Patch;
typedef struct { s32 count; Patch *patches; } PatchList;
typedef struct { s32 width; } MapInfo;
typedef struct { MapInfo *info; s32 unk4; PatchList *patches; } Map;
extern s32 D_8016DB6C;
extern Obj *D_801D40CC;
extern Map *D_8013B980;
extern ObjTypeTable *D_801DE97C;
void func_800673D8(u16 id, u16 mask, s32 enable, u16 *tilemap) {
    Obj *obj = D_801D40CC;
    Obj *objEnd = obj + D_8016DB6C;
    MapInfo *info = D_8013B980->info;
    PatchList *list = D_8013B980->patches;
    Patch *p;
    Patch *pEnd;
    s32 row;
    s32 col;
    u16 *dst;
    u16 *src;

    for (; obj < objEnd; obj++) {
        if (obj->active == 0) continue;
        if ((D_801DE97C->types[obj->type].category & mask) != id) continue;
        if (enable) obj->flags |= 1; else obj->flags &= ~1;
    }
    if (tilemap == 0 || enable == 0) return;
    p = list->patches;
    pEnd = p + list->count;
    for (; p < pEnd; p++) {
        if (p->id != id) continue;
        src = p->tiles;
        for (row = 0; row < p->rows; row++) {
            dst = tilemap + ((p->y + row) * (info->width + 20) + p->x);
            for (col = 0; col < p->cols; col++, dst++, src++) {
                if (*src != 0xFFFF) *dst = (*dst & 0x1CDF) | *src;
            }
        }
    }
}

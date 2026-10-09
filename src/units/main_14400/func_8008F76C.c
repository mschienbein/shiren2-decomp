#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { short pos[3]; char pad6[2]; short st[2]; u8 color[4]; } GeomVtx;
typedef struct { u32 unk0; u32 unk4; u32 count; u8 *indices; } GeomMesh;
typedef struct {
    char header[0x14];
    u32 unk14;
    u32 vtxCount;
    GeomVtx *vtx;
    u32 meshCount;
    GeomMesh *meshes;
} Geom;
void *func_80091450(u32);
void func_80091544(void *);
u8 *func_8006A810(u8 *, s32, s32);
void func_8008D3A0(void *, s32, void (*)(void), s32 (*)(void *, void *, void *), void (*)(void *));
s32 func_8008DF04(void *);
u32 func_8008E0C4(void *, void *, u32);
void func_8008F540(void);
s32 func_8008F548(void *, void *, void *);
void func_8008F6D4(void *record);
static inline u32 read_bytes(void *reader, u8 *base, u32 offset, u32 size) {
    func_8008E0C4(reader, base + offset, size);
    return size;
}
Geom *func_8008F76C(void *reader, s32 size) {
    s32 err = 0;
    u32 used;
    u32 i;
    u32 n; /* mesh table size, then the per-mesh triangle counter */
    Geom *geom;

    /* ODD_C: single-pass error block routing geometry, vertex and mesh allocation/parsing
     * failures to the shared destructor and free; it also shapes the prologue scheduling
     * and the original saved-register choice. */
    do {
        geom = func_80091450(sizeof(Geom));
        if (geom == 0) {
            err = -1;
            break;
        }
        func_8006A810((u8 *)geom, 0, sizeof(Geom));
        func_8008D3A0(geom, 0x47454F4D, func_8008F540, func_8008F548, func_8008F6D4);
        geom->unk14 = func_8008DF04(reader);
        geom->vtxCount = func_8008DF04(reader);
        used = 8;
        if (geom->vtxCount != 0) {
            geom->vtx = func_80091450(geom->vtxCount * sizeof(GeomVtx));
            if (geom->vtx == 0) {
                err = -1;
                break;
            }
            for (i = 0; i < geom->vtxCount; i++) {
                u16 pos[3];
                u16 color[4];
                u16 st[2];
                GeomVtx *v;
                func_8008E0C4(reader, pos, sizeof(pos));
                func_8008E0C4(reader, color, sizeof(color));
                func_8008E0C4(reader, st, sizeof(st));
                v = &geom->vtx[i];
                v->pos[0] = pos[0];
                v->pos[1] = pos[1];
                v->pos[2] = pos[2];
                v->color[0] = color[0];
                v->color[1] = color[1];
                v->color[2] = color[2];
                v->color[3] = color[3];
                v->st[0] = st[0];
                v->st[1] = st[1];
                used += sizeof(pos) + sizeof(color) + sizeof(st);
            }
        }
        geom->meshCount = func_8008DF04(reader);
        used += 4;
        if (geom->meshCount != 0) {
            n = geom->meshCount * sizeof(GeomMesh);
            geom->meshes = func_80091450(n);
            if (geom->meshes == 0) {
                err = -1;
                break;
            }
            func_8006A810((u8 *)geom->meshes, 0, n);
            for (i = 0; i < geom->meshCount; i++) {
                GeomMesh *mesh;
                mesh = &geom->meshes[i];
                mesh->unk4 = func_8008DF04(reader);
                used += 4;
                mesh->unk0 = func_8008DF04(reader);
                used += 4;
                mesh->count = func_8008DF04(reader);
                used += 4;
                mesh->indices = func_80091450(mesh->count * 3);
                if (mesh->indices == 0) {
                    err = -1;
                    break;
                }
                for (n = 0; n < mesh->count; n++) {
                    used += read_bytes(reader, mesh->indices, n * 3, 3);
                }
            }
            if (err != 0) {
                break;
            }
        }
        if (used != size) {
            err = -1;
        }
    } while (0);
    if (err != 0 && geom != 0) {
        func_8008F6D4(geom);
        func_80091544(geom);
        geom = 0;
    }
    return geom;
}

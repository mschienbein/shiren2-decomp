#include "common.h"
typedef unsigned char u8;
typedef struct Stream Stream;
typedef struct {
    u8 first[4], second[4], flag;
    u8 field9[4], fieldD[4], field11[4], field15[4];
} Material;
extern u32 func_8008E0C4(Stream *stream, void *dst, u32 length);
extern s32 func_8008E1E8(Stream *stream);
extern void func_8008FC04(u8 *color, float *rgba);

s32 func_8008FCDC(Stream *stream, Material *material)
{
    float first[4];
    float second[4];
    s32 discarded;
    float *rgba;
    s32 *unused;
    func_8008E0C4(stream, first, 0x10);
    func_8008E1E8(stream);
    func_8008FC04(material->first, first);
    rgba = second;
    func_8008E0C4(stream, rgba, 0x10);
    func_8008FC04(material->second, rgba);
    func_8008E0C4(stream, &material->flag, 1);
    func_8008E0C4(stream, material->field9, 4);
    func_8008E0C4(stream, material->fieldD, 4);
    func_8008E0C4(stream, material->field11, 4);
    func_8008E0C4(stream, material->field15, 4);
    unused = &discarded;
    func_8008E0C4(stream, unused, 4);
    func_8008E0C4(stream, unused, 4);
    func_8008E0C4(stream, unused, 4);
    func_8008E0C4(stream, unused, 4);
    return 0x41;
}

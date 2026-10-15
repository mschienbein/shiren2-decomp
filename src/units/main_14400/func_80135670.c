#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef struct {
    u8 pad00[0x14];
    u8 bytes_per_sample, channels;
    u16 width, height, x, y;
    u8 bits, shift;
    u8 pad20[4];
    s32 offsets[13];
    s32 raw_offsets[3];
} MovieHeader;
typedef struct { u32 mask, word; const u32 *next; } BitStream;
typedef struct { s16 left, right; } TreeNode;
typedef struct { s16 root; TreeNode nodes[512]; } Tree;
typedef void (*ColorConverter)(u16 *, u16 *, u16 *, u16 *);

extern ColorConverter D_801D2510;
extern s32 D_801D2514, D_801D2518, D_801D251C, D_801D2520;
extern s32 D_801D2524, D_801D2528, D_801D252C, D_801D2530;
extern s32 D_801D2534, D_801D2538, D_801D2540, D_801D2544;
extern u8 *D_801D253C;
extern u8 D_801D2548;
extern u8 *D_801CF64C[3];
extern BitStream D_801CF5A8[4], D_801CF5D8[3], D_801CF600[3], D_801CF628[3];
extern Tree D_801CFE70, D_801D0E90, D_801D0680, D_801CF660;
extern u16 D_801D1692;
extern s16 D_801D1694, D_801D1696;
extern s16 *D_801D1698, *D_801D169C, *D_801D16A0;
extern u8 D_801CEB40[];
extern void func_80133A68(u16 *, u16 *, u16 *, u16 *);
extern void func_80134000(u16 *, u16 *, u16 *, u16 *);
extern s32 func_80132050(BitStream *, Tree *);
extern void func_80132324(void);
extern void func_801328A8(void);
extern void func_80132C5C(u8 *);
extern void func_80135670(u16 *);


typedef struct { signed char first, second; } Sample5670;
typedef struct {
    Sample5670 *previous;
    Sample5670 *current;
    Sample5670 *next;
    Sample5670 samples[6];
} Channel;
/* Shared decoder state, also consumed by func_8013517C. Keep the complete
 * 0x18-byte channel views external: the preserved module BSS owns them and
 * its interior assembly labels. KMC fills the delay slots without local
 * definitions; forcing .bss ownership here would discard those live labels. */
extern Channel D_801D16A8;
extern Channel D_801D16C0;
extern Channel D_801D16D8;
extern Channel D_801D16F0;
extern void func_8013517C(u16 *out);

void func_80135670(u16 *out) {
    s32 rows;
    D_801D16D8.previous = D_801D16D8.current = ((Sample5670 *)D_801D169C);
    D_801D16D8.next = ((Sample5670 *)D_801D169C) + D_801D2530;
    D_801D16F0.previous = D_801D16F0.current = ((Sample5670 *)D_801D16A0);
    D_801D16F0.next = ((Sample5670 *)D_801D16A0) + D_801D2530;
    D_801D16A8.previous = D_801D16A8.current = ((Sample5670 *)D_801D1698);
    D_801D16A8.next = ((Sample5670 *)D_801D1698) + D_801D2524;
    if (D_801D2520) {
        D_801D16C0.previous = ((Sample5670 *)D_801D1698);
        D_801D16C0.current = ((Sample5670 *)D_801D1698) + D_801D2524;
        D_801D16C0.next = D_801D16C0.current + D_801D2524;
    }
    func_8013517C(out);
    out += D_801D251C;
    D_801D16D8.previous = ((Sample5670 *)D_801D169C);
    D_801D16F0.previous = ((Sample5670 *)D_801D16A0);
    if (D_801D2520) {
        D_801D16A8.current += D_801D2524;
        D_801D16A8.next += D_801D2524;
        D_801D16C0.previous += D_801D2524;
        D_801D16C0.current += D_801D2524;
        D_801D16C0.next += D_801D2524;
    } else {
        D_801D16A8.previous = ((Sample5670 *)D_801D1698);
    }
    for (rows = D_801D2534 - 2; rows > 0; rows--) {
        func_8013517C(out);
        out += D_801D251C;
        if (D_801D2520) {
            D_801D16A8.previous += D_801D2524;
            D_801D16A8.current += D_801D2524;
            D_801D16A8.next += D_801D2524;
            D_801D16C0.previous += D_801D2524;
            D_801D16C0.current += D_801D2524;
            D_801D16C0.next += D_801D2524;
        }
    }
    D_801D16D8.next = D_801D16D8.current;
    D_801D16F0.next = D_801D16F0.current;
    if (D_801D2520) {
        D_801D16C0.next = D_801D16C0.current;
    } else {
        D_801D16A8.next = D_801D16A8.current;
    }
    func_8013517C(out);
}

static inline void init_stream(BitStream *stream, const u32 *chunk)
{
    if (*chunk != 0) {
        stream->next = chunk + 1;
        stream->mask = 0;
    } else {
        stream->next = 0;
        stream->mask = 0;
    }
}

static inline void init_tree(BitStream *stream, const u32 *chunk, Tree *tree)
{
    if (*chunk != 0) {
        stream->next = chunk + 1;
        stream->mask = 0;
        if (tree != 0) {
            D_801D1692 = 0x100;
            tree->root = func_80132050(stream, tree);
        }
    } else {
        stream->next = 0;
        stream->mask = 0;
    }
}

void func_8013591C(void *source, void *output, s32 stride, void *workspace)
{
    u8 *data = source;
    s16 *frame = workspace;
    MovieHeader *header = source;
    s32 width, height, half_width, chroma_height, i, shift;
    s32 channel_stride, area;
    s32 channels;
    s32 value;
    D_801D2514 = stride;
    D_801D2518 = header->bytes_per_sample * 4;
    channel_stride = header->channels * (stride << 2);
    width = header->width;
    width = (u32)width >> 2;
    height = header->height;
    height = (u32)height >> 2;
    area = width * height;
    D_801D251C = channel_stride;
    channels = header->channels;
    /* ODD_C: Luma and chroma keep separate heights; their distinct lifetimes
     * preserve the original dimension-register allocation. */
    chroma_height = height;
    half_width = width >> 1;
    D_801D2524 = width;
    D_801D2528 = height;
    D_801D252C = area;
    D_801D2530 = half_width;
    D_801D2520 = channels == 2;
    if (D_801D2520) chroma_height >>= 1;
    D_801D2534 = chroma_height;
    D_801D2538 = half_width * chroma_height;
    D_801D2510 = D_801D2520 ? func_80134000 : func_80133A68;
    D_801D2548 = header->bits;
    if (D_801D2548 == 8) {
        D_801D2540 = 0x46;
        D_801D2544 = 0x26;
    } else {
        D_801D2540 = 0x26;
        D_801D2544 = 0x46;
    }
    D_801CF64C[0] = data + header->raw_offsets[0] + 4;
    D_801CF64C[1] = data + header->raw_offsets[1] + 4;
    D_801CF64C[2] = data + header->raw_offsets[2] + 4;
    init_tree(&D_801CF5A8[0], (u32 *)(data + header->offsets[0]), &D_801CFE70);
    init_stream(&D_801CF5A8[1], (u32 *)(data + header->offsets[1]));
    init_tree(&D_801CF5A8[2], (u32 *)(data + header->offsets[2]), &D_801D0E90);
    init_stream(&D_801CF5A8[3], (u32 *)(data + header->offsets[3]));
    init_stream(&D_801CF628[0], (u32 *)(data + header->offsets[7]));
    init_stream(&D_801CF628[1], (u32 *)(data + header->offsets[8]));
    init_stream(&D_801CF628[2], (u32 *)(data + header->offsets[9]));
    init_tree(&D_801CF600[0], (u32 *)(data + header->offsets[10]), &D_801D0680);
    init_stream(&D_801CF600[1], (u32 *)(data + header->offsets[11]));
    init_stream(&D_801CF600[2], (u32 *)(data + header->offsets[12]));
    init_tree(&D_801CF5D8[0], (u32 *)(data + header->offsets[4]), &D_801CF660);
    init_stream(&D_801CF5D8[1], (u32 *)(data + header->offsets[5]));
    init_stream(&D_801CF5D8[2], (u32 *)(data + header->offsets[6]));
    value = 0;
    shift = header->shift;
    D_801D1694 = 0x7F << shift;
    D_801D1696 = -(0x80 << shift);
    for (i = 0; i < 256; i++, value++) {
        /* ODD_C: Extract Q3 and unit values from one sign-positioned byte;
         * spelling the two precisions explicitly preserves the original shifts. */
        s32 code = (s32)((u32)value << 24);
        D_801D0680.nodes[i].left = code >> 21;
        D_801CF660.nodes[i].left = (code >> 24) * (1 << shift);
    }
    D_801D253C = D_801CEB40;
    D_801D1698 = frame;
    D_801D169C = frame + D_801D252C;
    D_801D16A0 = D_801D169C + D_801D2538;
    func_80132324();
    func_801328A8();
    func_80132C5C((u8 *)(D_801D1698 + (header->x + header->y * D_801D2524)));
    func_80135670(output);
}


extern u8 D_801D1F10[0x300];
extern u8 D_801D2210[0x300];
extern s32 D_801D1710[0x200];

void func_80135ED4(s32 mode) {
    s32 i;
    s32 level;
    s32 *recip;
    u32 alpha = (u8)mode >> 7;

    for (i = 0, level = -0x100; i < 0x300; i++) {
        if (level >= 0) {
            if (level < 0x100) {
                D_801D1F10[i] = (level >> 2) & 0x3E;
            } else {
                D_801D1F10[i] = 0x3E;
            }
        } else {
            D_801D1F10[i] = 0;
        }
        level++;
        /* FAKEMATCH: Retain the original byte read/store before the OR.
         * With gcc272-kmc26-mips2-unsigned, omission, a (u8) read cast, or
         * swapping the OR operands each gives 32 differing words and a
         * 204-byte function instead of 208; computing into a byte local
         * before storing gives 45 differing words. This re-store matches. */
        D_801D1F10[i] &= 0xFF;
        D_801D2210[i] = D_801D1F10[i] | alpha;
    }
    recip = D_801D1710;
    recip[0] = 0;
    for (i = 1; i < 0x200; i++) {
        recip[i] = 0x1000 / i;
    }
}

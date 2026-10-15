#include "common.h"
typedef unsigned char u8;
typedef struct {
    s32 handle_00; u32 state_04; s32 field_08; s32 timeout_0C; s32 offset_10;
    s32 write_14; s32 read_18; s32 height_1C; char lines_20[4][256];
} TextQueue80053D70;
extern u8 D_801398B2;
extern TextQueue80053D70 D_80161724[];
extern u8 D_80161B44;
extern s32 func_8005ECF8(char *dst, const char *fmt, void *args);
extern s32 func_80053C80(char *text);
extern void func_80053AE8(s32 index);
extern void func_80053D20(s32 index, char *text);
extern s32 func_80054B10(void);
/* The null-context registration calls func_80054258 with no arguments. */
extern s32 func_80081D60(s32 x, s32 y, s32 width, s32 height, void (*draw)(void));
extern void func_80054258(void);
extern void func_80082658(u32 index, s32 value);
extern void func_800838E0(s32 index, s32 value);
extern void func_800838B8(s32 index, unsigned short value);
extern void func_80054C10(u8 mode, char *text);
extern void func_800265E0(void *data, s32 length);
extern u32 func_80032D70(const char *text);
extern char *func_80083C90(char *dest, char *src);
extern s32 func_80083730(void);
extern void func_800836D4(s32 value);
extern s32 func_80053CC8(char *text, s32 count);
extern void func_800835AC(u32 index);
static __inline__ void reset_queue(TextQueue80053D70 *queue) {
    queue->read_18 = 0; queue->write_14 = 0; queue->state_04 = 0;
    func_800265E0(queue->lines_20, sizeof(queue->lines_20));
}
static __inline__ void advance_queue(TextQueue80053D70 *queue) {
    queue->write_14 = (queue->write_14 + 1) % 4;
    queue->lines_20[queue->write_14][0] = 0;
}
void func_80053D70(s32 index, s32 mode, s32 x, s32 y, s32 width, s32 height, char *format, void *args) {
    char combined[512];
    char formatted[512];
    s32 lines;
    TextQueue80053D70 *queue;
    func_8005ECF8(formatted, format, args);
    lines = func_80053C80(formatted);
    queue = &D_80161724[index];
    if (D_801398B2) {
        func_80053AE8(index);
        D_801398B2 = 0;
    }
    if ((queue->write_14 + 1) % 4 == queue->read_18) return;
    if (queue->handle_00 != -1) {
        height = queue->height_1C;
        func_80053D20(queue->handle_00, formatted);
    }
    if (func_80054B10() == 1) {
        func_80054C10((u8)mode, formatted);
        return;
    }
    if (queue->handle_00 == -1) {
        queue->handle_00 = func_80081D60(x, y, width, height, func_80054258);
        func_80082658(queue->handle_00, 1);
        func_800838E0(queue->handle_00, 0);
        if ((u32)(D_80161B44 - 2) < 2U) func_800838B8(queue->handle_00, 0);
        func_80053D20(queue->handle_00, formatted);
        queue->height_1C = height;
        reset_queue(queue);
    } else if (queue->state_04 >= 2U && func_80054B10() != 2) {
        if (D_80161B44 == 0) {
            func_80054C10((u8)mode, formatted);
            return;
        }
        if (lines != 0) {
            func_80054C10((u8)mode, formatted);
            return;
        }
    } else if (mode == 1) {
        if (D_80161B44 != 0) reset_queue(queue);
        else advance_queue(queue);
    } else {
        s32 length = func_80032D70(queue->lines_20[queue->write_14]);
        if (D_80161B44 == 0) {
            queue->lines_20[queue->write_14][length] = '\n';
            length++;
            queue->lines_20[queue->write_14][length] = 0;
        }
        func_80083C90(combined, queue->lines_20[queue->write_14]);
        func_80083C90(combined + length, formatted);
        func_80053D20(queue->handle_00, combined);
        lines = func_80053C80(combined);
        if (lines >= height / 2 && lines < height - 1) {
            s32 old_slot;
            s32 split;
            s32 padding;
            lines -= height / 2;
            lines++;
            queue->offset_10 = (height - lines * 2) * 8;
            if (D_80161B44 == 2) queue->state_04 = 3;
            else {
                if (func_80083730() && D_80161B44 == 0) {
                    queue->state_04 = 1;
                    func_800836D4(1);
                    func_80054C10((u8)mode, formatted);
                    return;
                }
                queue->state_04 = 2;
            }
            old_slot = queue->write_14;
            queue->write_14 = (old_slot + 1) % 4;
            split = func_80053CC8(queue->lines_20[old_slot], lines);
            func_80083C90(queue->lines_20[queue->write_14], &queue->lines_20[old_slot][split + 1]);
            queue->lines_20[old_slot][split] = 0;
            for (padding = 0; padding < height / 2 - lines; padding++) {
                combined[padding] = '\n';
            }
            func_80083C90(combined + padding, queue->lines_20[old_slot]);
            func_80083C90(queue->lines_20[old_slot], combined);
        } else if (lines >= height - 1) advance_queue(queue);
    }
    queue->field_08 = 0;
    queue->timeout_0C = 120;
    func_80083C90(queue->lines_20[queue->write_14] + func_80032D70(queue->lines_20[queue->write_14]), formatted);
    func_80053D20(queue->handle_00, queue->lines_20[queue->write_14]);
    func_800835AC(queue->handle_00);
}

#include "common.h"

typedef struct { u32 word0, word1; } Gfx;
typedef struct {
    short x, y, z;
    unsigned short flags;
    short s, t;
    unsigned char r, g, b, a;
} Vertex;
typedef struct {
    unsigned char unk00[6];
    unsigned char width;
    unsigned char height;
    signed char x_origin;
    signed char y_origin;
} Sprite;
extern void *func_800705F4(s32);

Gfx *func_80071420(Gfx *display_list, void *image, Sprite *sprite,
                   s32 x, s32 y, s32 z, Vertex **vertex_buffer) {
    u32 line_bytes = ((sprite->width >> 1) + 7) & 0xF8;
    s32 strip_height = 0x800U / line_bytes - 1;
    s32 vertex_count = ((sprite->height + strip_height - 1) / strip_height) * 2;
    s32 allocate = *vertex_buffer == 0;
    Vertex *vertex;
    Gfx *image_command;
    Gfx *load_tile_command;
    Gfx *render_tile_command;
    s32 loaded;
    s32 remaining;
    s32 row;
    s32 height;
    s32 index;
    s32 next_row;
    u32 top;
    u32 bottom;
    s32 left;
    s32 right;
    s32 screen_y;
    s32 texture_width;

    vertex_count += ((vertex_count + 29) / 30) * 2;
    if (allocate) {
        vertex = func_800705F4(vertex_count);
        if (vertex == 0) {
            return display_list;
        }
        *vertex_buffer = vertex;
    } else {
        vertex = *vertex_buffer;
    }

    image_command = display_list++;
    image_command->word0 = ((((sprite->width + 1) >> 1) - 1) & 0xFFF) | 0xFD480000;
    image_command->word1 = (u32)image;
    load_tile_command = display_list++;
    load_tile_command->word0 = (line_bytes << 6) | 0xF5480000;
    load_tile_command->word1 = 0x07080200;
    render_tile_command = display_list++;
    render_tile_command->word0 = (line_bytes << 6) | 0xF5400000;
    render_tile_command->word1 = 0x00080200;
    index = 0;
    loaded = 0;
    remaining = 0;
    screen_y = sprite->height - sprite->y_origin + y;
    texture_width = sprite->width << 6;
    left = x - sprite->x_origin;
    right = left + sprite->width;
    row = 0;
    while (row < sprite->height) {
        height = sprite->height - row;
        if (strip_height < height) {
            height = strip_height;
        }
        if (remaining == 0) {
            remaining = vertex_count - loaded;
            if (remaining >= 33) {
                remaining = 32;
            }
            {
                Gfx *command = display_list++;
                command->word0 = 0xE7000000;
                command->word1 = 0;
            }
            {
                Gfx *command = display_list++;
                u32 count_bits;
                u32 end_bits;
                loaded += remaining;
                index = 0;
                count_bits = (remaining & 0xFF) << 12;
                end_bits = ((remaining & 0x7F) * 2) | 0x01000000;
                command->word0 = count_bits | end_bits;
                command->word1 = (u32)vertex;
            }
            if (allocate) {
                vertex->x = left;
                vertex->y = screen_y;
                vertex->z = z;
                vertex->flags = 0;
                vertex->s = 0;
                vertex->t = row << 6;
                vertex->r = 255;
                vertex->g = 255;
                vertex->b = 255;
                vertex->a = 255;
                vertex++;
                vertex->x = right;
                vertex->y = screen_y;
                vertex->z = z;
                vertex->flags = 0;
                vertex->s = texture_width;
                vertex->t = row << 6;
                vertex->r = 255;
                vertex->g = 255;
                vertex->b = 255;
                vertex->a = 255;
                vertex++;
            } else {
                vertex += 2;
            }
            remaining -= 2;
        }

        {
            Gfx *command = display_list++;
            command->word0 = 0xE6000000;
            command->word1 = 0;
        }
        {
            Gfx *command = display_list++;
            top = (row * 4) & 0xFFF;
            next_row = row + height;
            command->word0 = top | 0xF4000000;
            bottom = (next_row * 4) & 0xFFF;
            command->word1 = 0x07000000 | ((((sprite->width - 1) * 2) & 0xFFC) << 12) | bottom;
        }
        {
            Gfx *command = display_list++;
            command->word0 = 0xE7000000;
            command->word1 = 0;
        }
        if (height == sprite->height - row) {
            Gfx *command = display_list++;
            command->word0 = top | 0xF2000000;
            command->word1 = ((((sprite->width - 1) * 4) & 0xFFF) << 12) | (((next_row - 1) * 4) & 0xFFF);
        } else {
            Gfx *command = display_list++;
            command->word0 = top | 0xF2000000;
            command->word1 = ((((sprite->width - 1) * 4) & 0xFFF) << 12) | bottom;
        }
        screen_y -= height;
        row += height;
        if (allocate) {
            vertex->x = left;
            vertex->y = screen_y;
            vertex->z = z;
            vertex->flags = 0;
            vertex->s = 0;
            vertex->t = row << 6;
            vertex->r = 255;
            vertex->g = 255;
            vertex->b = 255;
            vertex->a = 255;
            vertex++;
            vertex->x = right;
            vertex->y = screen_y;
            vertex->z = z;
            vertex->flags = 0;
            vertex->s = texture_width;
            vertex->t = row << 6;
            vertex->r = 255;
            vertex->g = 255;
            vertex->b = 255;
            vertex->a = 255;
            vertex++;
        } else {
            vertex += 2;
        }
        {
            Gfx *command = display_list++;
            command->word0 = ((((index + 1) * 2) & 0xFF) << 16) | (((index * 2) & 0xFF) << 8) | (((index + 2) * 2) & 0xFF) | 0x06000000;
            command->word1 = ((((index + 1) * 2) & 0xFF) << 16) | ((((index + 2) * 2) & 0xFF) << 8) | (((index + 3) * 2) & 0xFF);
        }
        index += 2;
        remaining -= 2;
    }
    return display_list;
}

#include "common.h"

typedef unsigned char u8;

/* Sprite tint: written either as one word or as r, g, b, a bytes. */
typedef union { s32 word; struct { u8 r, g, b, a; } bytes; } Color;
/* Complete 0x50-byte sprite record (same layout as func_800581E8's view). */
typedef struct {
    u8 pad_00[0xC];
    s32 mode_0C, mode_10;
    Color color;
    u8 pad_18[4];
    u8 combine[16];
    float x, y, scale_x, scale_y;
    u8 pad_3C[4];
    s32 layer, image, palette;
    u8 enabled, alpha, field_4E, field_4F;
} Sprite;
typedef struct { s32 x, y; } Point;

/* Selected row (-1: none) and the frame counter of its blinking cursor. */
s32 D_8013A2A8 = -1, D_8013A2AC = 0;
extern s32 D_8013A2B0;
extern s32 D_8013A2B4[5];
extern Point D_8013A2C8[5][4];
extern char D_801630D8[];
extern Sprite *D_80163100;
s32 func_800424EC(s32 id);
s32 func_80042564(s32 row);
s32 func_80042594(s32 row);
s32 func_800425C4(s32 row, s32 col);
s32 func_8004260C(s32 row, s32 column);
s32 func_8004264C(s32 arg);
s32 func_80042684(s32 value);
s32 func_800426B4(void);
s32 func_80042700(void);
void func_800584EC(Sprite *sprite);
s32 func_80070598(void *object, void *value);

void *func_800570C0(void *arg) {
    /* states[5] is real but dead: the ROM performs five stores that are never read.
       s1=sp+0x10 at 0x800570E0; `sw v0,0(s1)` at 0x80057120 (ROM 0x2A160); s1+=4 at
       0x8005712C; five-iteration bound at 0x80057134/0x80057138; s1 reused at 0x8005720C. */
    s32 states[5],counts[5],unlocked[5];
    u8 pulse;
    s32 allUnlocked,i,j,spriteCount=0;
    Sprite *sprite;
    for(i=0;i<5;i++) {
        unlocked[i]=func_800424EC(i);
        states[i]=func_80042564(i);
        counts[i]=func_80042594(i);
    }
    allUnlocked=1;
    for(i=0;i<5;i++) if(unlocked[i]<4) allUnlocked=0;
    i=func_800426B4();
    if(i<0 || i!=D_8013A2A8) D_8013A2AC=0;
    D_8013A2A8=i;
    if(D_8013A2B0<32) pulse=32-D_8013A2B0;
    else pulse=D_8013A2B0-32;
    pulse = pulse==32 ? 255 : pulse<<3;
    sprite=&D_80163100[spriteCount++];
    func_800584EC(sprite);
    sprite->layer=50;
    sprite->image=0x170;
    sprite->palette=0x20;
    sprite->enabled=6;
    sprite->alpha=0xFE;
    sprite->x=162.0f;
    sprite->y=89.0f;
    func_80070598(D_801630D8,sprite);
    if(!func_80042700()) {
        sprite=&D_80163100[spriteCount++];
        func_800584EC(sprite);
        sprite->layer=50;
        sprite->palette=0x21;
        sprite->image=0x170;
        sprite->enabled=13;
        sprite->alpha=0xFE;
        sprite->x=186.0f;
        sprite->y=71.0f;
        func_80070598(D_801630D8,sprite);
    }
    if(D_8013A2A8>=0) {
        s32 selected;
        s32 row;
        sprite=&D_80163100[spriteCount];
        func_800584EC(sprite);
        row=D_8013A2A8;
        sprite->palette=0x1A;
        sprite->enabled=6;
        sprite->alpha=0xFE;
        sprite->combine[0]=3; sprite->combine[1]=1; sprite->combine[2]=10; sprite->combine[3]=1;
        sprite->combine[4]=7; sprite->combine[5]=7; sprite->combine[6]=7; sprite->combine[7]=1;
        sprite->combine[8]=3; sprite->combine[9]=1; sprite->combine[10]=10; sprite->combine[11]=1;
        sprite->combine[12]=7; sprite->combine[13]=7; sprite->combine[14]=7; sprite->combine[15]=1;
        spriteCount++;
        sprite->image=0x170;
        sprite->x=20.0f;
        sprite->y=(float)(row*41+16);
        if(D_8013A2AC&1) sprite->color.word=-128;
        else sprite->color.word=-256;
        func_80070598(D_801630D8,sprite);
        selected=func_80042684(D_8013A2A8);
        if(selected>=0 && D_8013A2AC>=8) {
            sprite=&D_80163100[spriteCount++];
            func_800584EC(sprite);
            sprite->x=(float)(selected*18+60);
            sprite->y=(float)(D_8013A2A8*41+26);
            sprite->layer=60;
            sprite->image=0x170;
            sprite->palette=func_8004264C(D_8013A2A8)+0x1C;
            sprite->enabled=D_8013A2A8+7;
            sprite->alpha=0xFE;
            func_80070598(D_801630D8,sprite);
        }
    }
    for(i=0;i<5;i++) {
        s32 hasSelection=func_80042684(i)>=0;
        if(counts[i]>0 || (i==D_8013A2A8 && hasSelection)) {
            sprite=&D_80163100[spriteCount++];
            func_800584EC(sprite);
            sprite->y=(float)(i*41+24);
            sprite->image=0x170;
            sprite->palette=i+0x14;
            sprite->enabled=12;
            sprite->alpha=0xFE;
            sprite->x=58.0f;
            func_80070598(D_801630D8,sprite);
        }
        for(j=0;j<counts[i];j++) {
            s32 icon;
            sprite=&D_80163100[spriteCount++];
            func_800584EC(sprite);
            sprite->image=0x170;
            sprite->x=162.0f;
            sprite->y=89.0f;
            sprite->palette=D_8013A2B4[i]+j;
            sprite->enabled=i;
            sprite->alpha=0xFE;
            if(j>=unlocked[i]) {
                sprite->combine[0]=1; sprite->combine[1]=31; sprite->combine[2]=3; sprite->combine[3]=31;
                sprite->combine[4]=1; sprite->combine[5]=7; sprite->combine[6]=3; sprite->combine[7]=7;
                sprite->combine[8]=1; sprite->combine[9]=31; sprite->combine[10]=3; sprite->combine[11]=31;
                sprite->combine[12]=1; sprite->combine[13]=7; sprite->combine[14]=3;
                sprite->mode_0C=0x4041C8;
                sprite->mode_10=0x1041C8;
                sprite->combine[15]=7;
                sprite->color.bytes.r=255;
                sprite->color.bytes.g=255;
                sprite->color.bytes.b=255;
                sprite->color.bytes.a=pulse;
            }
            func_80070598(D_801630D8,sprite);
            icon=func_800425C4(i,j);
            sprite=&D_80163100[spriteCount++];
            func_800584EC(sprite);
            sprite->x=(float)(j*18+60);
            sprite->y=(float)(i*41+26);
            sprite->layer=50;
            sprite->image=0x170;
            sprite->palette=icon+0x1C;
            sprite->enabled=i+7;
            sprite->alpha=0xFE;
            func_80070598(D_801630D8,sprite);
            if(func_8004260C(i,j)>0) {
                sprite=&D_80163100[spriteCount++];
                func_800584EC(sprite);
                sprite->x=(float)D_8013A2C8[i][j].x;
                sprite->y=(float)D_8013A2C8[i][j].y;
                sprite->layer=100;
                sprite->image=0x170;
                sprite->palette=0x1B;
                sprite->enabled=5;
                sprite->alpha=0xFE;
                func_80070598(D_801630D8,sprite);
            }
        }
    }
    if(allUnlocked) {
        sprite=&D_80163100[spriteCount];
        func_800584EC(sprite);
        sprite->layer=200;
        sprite->image=0x170;
        sprite->palette=0x19;
        sprite->enabled=3;
        sprite->alpha=0xFE;
        sprite->x=216.0f;
        sprite->y=146.0f;
        func_80070598(D_801630D8,sprite);
    }
    D_8013A2AC=(D_8013A2AC+1)%16;
    D_8013A2B0=(D_8013A2B0+1)%64;
    return arg;
}

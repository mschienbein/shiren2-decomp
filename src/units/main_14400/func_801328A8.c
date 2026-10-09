#include "common.h"

typedef unsigned char u8;

/* MSB-first bit reader over a word stream (same layout as func_80132050's). */
typedef struct BitStream { u32 mask; u32 word; const u32 *next; } BitStream;

/* Interleaved Y, U and V byte planes (one byte every two); module bss shared
 * with func_80132324 and the decoder setup code. */
extern u8 *D_801D1698;
extern u8 *D_801D169C;
extern u8 *D_801D16A0;
extern s32 D_801D2530;
extern s32 D_801D2534;
extern s32 D_801D2520;
/* Value and run-length bit streams of the three planes (Y, U, V). */
extern BitStream D_801CF5D8[3];
extern BitStream D_801CF628[3];
extern s32 func_80132684(u8 *run, BitStream *values, BitStream *runs);

/*
 * Decode the predictive Y/U/V planes: the first row predicts from the left
 * neighbour, later rows from the average of the left and upper neighbours;
 * D_801D2520 adds a luma-only tail of the same length after each row.
 */
void func_801328A8(void) {
    u8 state_y, state_u, state_v;
    s32 rows;
    u8 *y = D_801D1698;
    u8 *u = D_801D169C;
    u8 *previous_u = u;
    u8 *v = D_801D16A0;
    u8 *previous_v = v;
    u8 *previous_y = y;
    s32 count = D_801D2530;
    s32 predictor_v = 0;
    s32 predictor_u = 0;
    s32 predictor_y = 0;
    s32 row_predictor_y;
    state_v = 0;
    state_u = 0;
    state_y = 0;
    while (count > 0) {
        s32 first = predictor_y + func_80132684(&state_y, D_801CF5D8, D_801CF628);
        *y = first;
        y += 2;
        predictor_y = first + func_80132684(&state_y, D_801CF5D8, D_801CF628);
        *y = predictor_y;
        y += 2;
        predictor_u += func_80132684(&state_u, &D_801CF5D8[1], &D_801CF628[1]);
        *u = predictor_u;
        u += 2;
        predictor_v += func_80132684(&state_v, &D_801CF5D8[2], &D_801CF628[2]);
        *v = predictor_v;
        v += 2;
        count--;
    }
    if (D_801D2520) {
        count = D_801D2530;
        predictor_y = *previous_y;
        /* FAKEMATCH: this mask has no semantic effect after the u8 load.
         * Combine removes it, but it raises predictor allocation priority
         * (s2 ahead of the row pointer). Removing both masks or folding them
         * into the initializers each misses 28 words; split assignments in
         * the loop bodies also change the code. */
        predictor_y &= 0xFF;
        while (count > 0) {
            s32 first = predictor_y + func_80132684(&state_y, D_801CF5D8, D_801CF628);
            s32 left;
            s32 second;
            u32 above;
            previous_y += 2;
            left = *previous_y;
            *y = first;
            y += 2;
            second = (left + (u8)first) >> 1;
            second += func_80132684(&state_y, D_801CF5D8, D_801CF628);
            previous_y += 2;
            above = *previous_y;
            *y = second;
            y += 2;
            above += (u8)second;
            predictor_y = above >> 1;
            count--;
        }
    }
    for (rows = D_801D2534 - 1; rows > 0; rows--) {
        predictor_u = *previous_u;
        predictor_v = *previous_v;
        row_predictor_y = *previous_y;
        count = D_801D2530;
        while (count > 0) {
            s32 first = row_predictor_y + func_80132684(&state_y, D_801CF5D8, D_801CF628);
            s32 left;
            s32 second;
            s32 delta;
            u32 above;
            previous_y += 2;
            left = *previous_y;
            *y = first;
            y += 2;
            second = (left + (u8)first) >> 1;
            delta = func_80132684(&state_y, D_801CF5D8, D_801CF628);
            previous_y += 2;
            previous_u += 2;
            previous_v += 2;
            above = *previous_y;
            second += delta;
            *y = second;
            above += (u8)second;
            row_predictor_y = above >> 1;
            first = predictor_u + func_80132684(&state_u, &D_801CF5D8[1], &D_801CF628[1]);
            above = *previous_u;
            *u = first;
            above += (u8)first;
            predictor_u = above >> 1;
            first = predictor_v + func_80132684(&state_v, &D_801CF5D8[2], &D_801CF628[2]);
            above = *previous_v;
            u += 2;
            y += 2;
            *v = first;
            v += 2;
            above += (u8)first;
            predictor_v = above >> 1;
            count--;
        }
        if (D_801D2520) {
            count = D_801D2530;
            predictor_y = *previous_y;
            /* FAKEMATCH: the same allocation-only mask and failed alternatives
             * as the first tail loop: no mask or initializer mask misses 28
             * words; split loop-body assignments also change the code. */
            predictor_y &= 0xFF;
            while (count > 0) {
                s32 first = predictor_y + func_80132684(&state_y, D_801CF5D8, D_801CF628);
                s32 left;
                s32 second;
                u32 above;
                previous_y += 2;
                left = *previous_y;
                *y = first;
                y += 2;
                second = (left + (u8)first) >> 1;
                second += func_80132684(&state_y, D_801CF5D8, D_801CF628);
                previous_y += 2;
                above = *previous_y;
                *y = second;
                y += 2;
                above += (u8)second;
                predictor_y = above >> 1;
                count--;
            }
        }
    }
}

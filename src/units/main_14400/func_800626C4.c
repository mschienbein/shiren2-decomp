#include "common.h"
extern u32 D_8016DB18;
extern void *D_8013B80C; /* func_800612D4 stores a func_8006A8D8 allocation result */
typedef struct Node Node; /* relocated node; layout owned by its users (see func_80064378) */
extern Node *D_8016DB50;
extern s32 D_801D2BF4;
extern s32 D_8016DB1C, D_8016DB20; /* index words set by func_80062804; passed by address */
extern s32 func_80072D54(void);
extern void func_8006CD14(void), func_800649F8(void *), func_80066A28(void *, void *), func_80067628(void *), func_80069A68(void *);
void func_800626C4(void){
  if (D_8013B80C != 0) {
    if (!func_80072D54()) func_8006CD14();
    D_8016DB50 = 0; D_801D2BF4 = 1;
    switch (D_8016DB18) {
    case 1: func_800649F8(&D_8016DB1C); break;
    case 2: func_80066A28(&D_8016DB1C, &D_8016DB20); break;
    case 3: func_80067628(&D_8016DB1C); break;
    case 4: func_80069A68(&D_8016DB1C); break;
    }
  } }

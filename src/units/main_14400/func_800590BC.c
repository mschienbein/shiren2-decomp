#include "common.h"
typedef struct { s32 words[8]; } Settings;
typedef struct { s32 key; Settings settings; } Entry;
extern Entry D_8013B2A4[8];
extern Settings D_8013B3C4[];
extern Settings D_8013B5C4;
extern s32 func_800627D4(void);
extern void func_80059728(Settings *settings);
extern s32 func_800627C4(void);
extern void func_8005ABBC(Settings *settings, s32 a, s32 b);
void func_800590BC(void)
{
    Settings local;
    s32 key = func_800627D4();
    Settings *settings = &local;
    Entry *entry;
    func_80059728(&local);
    switch ((u32)func_800627C4()) {
    case 1:
        for (entry = D_8013B2A4; entry < D_8013B2A4 + 8; entry++) {
            if (entry->key == key) {
                settings = &entry->settings;
                break;
            }
        }
        break;
    case 2:
        settings = &D_8013B3C4[key];
        break;
    case 3:
        settings = &D_8013B5C4;
        break;
    }
    func_8005ABBC(settings, 0, 0);
}

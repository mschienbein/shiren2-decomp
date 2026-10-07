#include "common.h"

/* This setter only forwards the table; the audio loop owns its method layout. */
typedef struct AudioDriverTable AudioDriverTable;
extern AudioDriverTable *D_80148AAC;

void func_8012A9CC(AudioDriverTable *value) {
    D_80148AAC = value;
}

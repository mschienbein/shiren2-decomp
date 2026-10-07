/* Partial views: byte offsets are observed; remaining layout is unresolved. */
typedef struct {
    unsigned char unk00[0x1E];
    unsigned char unk1E;
} UnkFunc800A6E50Arg0;

typedef struct {
    unsigned char unk00;
    unsigned char unk01;
    unsigned char unk02;
} UnkFunc800A6E50Arg1;

extern int func_800EC630(UnkFunc800A6E50Arg0 *arg0,
                       UnkFunc800A6E50Arg1 *arg1);

int func_800A6E50(UnkFunc800A6E50Arg0 *arg0,
                  UnkFunc800A6E50Arg1 *arg1) {
    int result;

    if (!((arg0->unk1E >> 2) & 1)) {
        result = arg1->unk02 & 0x10;
        result = result == 0;
    } else {
        result = func_800EC630(arg0, arg1);
    }
    return result;
}

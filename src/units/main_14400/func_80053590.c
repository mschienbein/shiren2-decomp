/*
 * Observed byte/halfword view only. This is not a recovered allocation/type.
 * The halfword's historical signedness is unresolved; writing zero has the
 * same bits. Byte 8 and every field not named below remain untouched.
 */
typedef struct {
    unsigned char unseen00[4];
    unsigned short field04;
    unsigned char field06;
    unsigned char field07;
    unsigned char untouched08;
    unsigned char field09;
} ObservedFields_80053590;

void func_80053590(ObservedFields_80053590 *record)
{
    record->field07 = 0;
    record->field04 = 0;
    record->field06 = 0;
    record->field09 = 0;
}

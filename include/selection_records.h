#ifndef SHIREN2_SELECTION_RECORDS_H
#define SHIREN2_SELECTION_RECORDS_H

/*
 * Selection state records (one definition for every C++ TU that uses them). Same layouts
 * as the C views of these objects.
 */

/* 12-byte selection record D_80142F18: u16 field_06 at +6, mode at +8 (0x80142F20),
 * signed coordinates at +9/+10 and signed status at +11. */
struct SelectionRecord {
    unsigned char kind, variant, field_02, flags, row, field_05;
    unsigned short field_06;
    unsigned char mode;
    signed char coordinates[2], status;
};

/* 11-byte saved selection D_80142F24: signed result at +9. */
struct SelectionSave {
    unsigned char index, count, previous, field_03, masks[2], field_06, previous_count, field_08;
    signed char result;
    unsigned char field_0A;
};

#endif

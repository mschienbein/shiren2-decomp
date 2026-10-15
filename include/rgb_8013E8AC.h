#ifndef SHIREN2_RGB_8013E8AC_H
#define SHIREN2_RGB_8013E8AC_H

/* D_8013E8AC is loaded main-image data (ROM 0x1118EC, RAM 0x8013E8AC).
 * The three consumers index by 3 and load unsigned bytes at +0/+1/+2,
 * then pack them into the R/G/B channels of a graphics command.
 * This is the complete element view, not a recovered array bound.
 * The existing unsized extern and assembly storage ownership are preserved.
 */
typedef struct Rgb_8013E8AC {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Rgb_8013E8AC;

typedef char Rgb_8013E8AC_size_check[(sizeof(Rgb_8013E8AC) == 3) ? 1 : -1];

extern Rgb_8013E8AC D_8013E8AC[];

#endif

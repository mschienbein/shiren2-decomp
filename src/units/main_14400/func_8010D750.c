#include "common.h"
extern void func_8010CCD0(unsigned char *);
/* Unit-family slot +0x54 setKind(self, kind): the kind byte stored at +1 is unsigned char. */
void func_8010D750(unsigned char *arg, unsigned char value) { arg[1]=value; func_8010CCD0(arg); arg[0xD]=0; arg[0xF]=0; }

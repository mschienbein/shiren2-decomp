#include "common.h"
typedef struct { short delta; short idx; void (*fn)(void *, s32); } VEnt;
typedef struct { VEnt e0; VEnt dtor; } VT;
typedef struct { char pad[8]; VT *vt; } Obj;
/* Payload objects forwarded to func_8011D568 (source read at +0x1E, target handed to
   func_800A7ADC, unit handed to func_801124F8); this TU only forwards the addresses. */
typedef struct { s32 type; void *source; void *target; char pad[0x10]; void *unit; } Ev;
extern void func_8011D568(Obj *, void *source, void *target, void *unit);
extern void func_800D3650(Obj *);
extern s32 func_8011276C(Obj *, Ev *);
s32 func_8011D4C0(Obj *self, Ev *ev){
  switch (ev->type) {
  case 0x12:
    func_8011D568(self, ev->source, ev->target, ev->unit);
    func_800D3650(self);
    if (self != 0) self->vt->dtor.fn((char *)self + self->vt->dtor.delta, 3);
    return 1;
  case 0x13:
    func_8011D568(self, ev->source, ev->target, ev->unit);
    return 1;
  }
  return func_8011276C(self, ev); }

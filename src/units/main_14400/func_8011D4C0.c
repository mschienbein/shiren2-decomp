#include "common.h"
#include "item_records.h"
/* Payload objects forwarded to func_8011D568 (source read at +0x1E, target handed to
   func_800A7ADC, unit handed to func_801124F8); this TU only forwards the addresses. */
typedef struct { s32 type; void *source; void *target; char pad[0x10]; void *unit; } Ev;
extern void func_8011D568(Item *, void *source, void *target, void *unit);
extern void func_800D3650(Item *);
extern s32 func_8011276C(Item *, Ev *);
s32 func_8011D4C0(Item *self, Ev *ev){
  switch (ev->type) {
  case 0x12:
    func_8011D568(self, ev->source, ev->target, ev->unit);
    func_800D3650(self);
    if (self != 0) self->field08->method0C((char *)self + self->field08->delta08, 3);
    return 1;
  case 0x13:
    func_8011D568(self, ev->source, ev->target, ev->unit);
    return 1;
  }
  return func_8011276C(self, ev); }

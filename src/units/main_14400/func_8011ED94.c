#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Point;
typedef struct { u8 value; } Dir;
typedef struct { u8 pad_00[0x10]; short adjust_10; short pad_12; s32 (*method_14)(void *); } Methods;
typedef struct Actor { Point position; u8 pad_08[0x14]; u16 field_1C; u8 pad_1E[6]; const Methods *field_24; } Actor;
typedef struct { Point position; u8 direction; s32 limit; s32 count; } Ray;
typedef struct { void *field_00; u32 field_04, field_08; u16 field_0C, field_0E; u8 field_10; u8 pad_11[7]; } Damage;
extern s32 func_80049CB4(s32 id, ...);
extern s32 func_800A251C(Point *a, Point *b);
extern void *func_800A2594(Point *out, void *position, Dir direction);
extern void func_800A2758(Point *position, Dir direction);
extern s32 func_800A4404(Actor *actor, Point *position, Actor **hit);
extern s32 func_800A4754(Actor *actor, void *position, Dir *direction);
extern void func_800A58FC(void *actor, Point *position);
extern void func_800A59A4(Actor *actor);
extern s32 func_800A6218(Actor *actor, Point *position, s32 kind);
extern void func_800A7ADC(Actor *actor, Damage *damage);
extern void func_800A7BA4(Actor *actor, s32 kind);
extern s32 func_800B56F0(void *position);
extern u16 func_800B5768(void *position);
extern void func_800C25D0(Ray *iterator, Point *position, u8 *direction, s32 count);
extern void *func_800C2758(void *out, void *iterator);
extern void func_80136910(Damage *damage, void *source, u32 amount, u32 kind, u32 flags);
static inline s32 inverted(s32 value) { return value ^ 1; }
static inline Point *copy_point(Point *out, Point *in) {
 out->x = in->x;
 out->y = in->y;
 return out;
}
static inline s32 ray_active(Ray *ray) { return ray->count < ray->limit; }
static inline s32 path_finished(s32 distance) { return distance < 2; }
static inline void init_damage(Damage *damage, void *source, u16 amount) {
 func_80136910(damage, source, (short)amount, 0x22, 0x808);
}
/* Trap apply slot +0x54 supplies five pointers; self and attacker are unused here. */
void func_8011ED94(void *self, void *source, Actor *actor, Dir *direction, void *attacker) {
 Point origin;
 Ray ray;
 Point destination;
 Point next;
 Point step;
 Damage damage;
 Dir reverse;
 Actor *hit;
 s32 distance;
 if (actor->field_1C & 2) return;
 copy_point(&origin, &actor->position);
 reverse.value = (direction->value + 4) & 7;
 func_800A2594(&destination, &origin, reverse);
 func_800C25D0(&ray, &destination, &reverse.value, 0xFF);
 distance = 0;
 copy_point(&destination, &origin);
 for (;;) {
  if (!ray_active(&ray)) break;
  if (inverted(func_800A4754(actor, &destination, &reverse))) break;
  func_800C2758(&next, &ray);
  if (inverted(func_800A4404(actor, &next, &hit))) break;
  destination = next;
  distance++;
 }
 if (func_800A251C(&destination, &origin)) return;
 func_800A59A4(actor);
 step.x = origin.x;
 step.y = origin.y;
 for (;;) {
  if (path_finished(distance--)) break;
  func_800A2758(&step, reverse);
  if (func_800B56F0(&step)) {
   func_80049CB4(0x1019, actor, &origin, &step);
   func_80049CB4(6);
   func_80049CB4(0x116, &step);
   func_80049CB4(7);
   init_damage(&damage, source, func_800B5768(&step));
   actor->field_1C |= 0x100;
   func_80049CB4(6);
   func_800A7ADC(actor, &damage);
   func_80049CB4(7);
   actor->field_1C &= ~0x100;
   if (actor->field_24->method_14((u8 *)actor + actor->field_24->adjust_10)) {
    func_800A58FC(actor, &step);
    return;
   }
   origin = step;
  }
 }
 func_80049CB4(0x1019, actor, &origin, &destination);
 func_800A6218(actor, &destination, 0);
 func_800A7BA4(actor, 7);
}

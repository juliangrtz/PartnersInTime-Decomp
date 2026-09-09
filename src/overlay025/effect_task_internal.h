#ifndef PIT_OVERLAY025_EFFECT_INTERNAL_H
#define PIT_OVERLAY025_EFFECT_INTERNAL_H
#include <game/overlay025_effect_task.h>
extern "C" {
#include <game/battle_global_properties.h>
#include <game/battle_hit.h>
extern u8 *gBattleContext;
#define OVERLAY25_PROJECTILE_COUNTS ((u32 *)(gBattleContext + 27108))
/* This shared table holds actor IDs during projectile preparation. */
#define OVERLAY25_PROJECTILE_TARGETS ((u32 *)(gBattleContext + 27108))
extern void func_ov002_02076178(int);
extern int func_ov002_0206c148(u16, BattleModel *, s16, s16, s16, int);
extern void func_ov025_020c2ee4(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
extern void func_ov025_020c34c4(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
extern void func_ov025_020c3a08(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
extern void func_ov025_020c4084(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
extern void func_ov025_020c4fd8(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
extern void func_ov025_020ca858(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
extern void func_ov025_020cbbb0(Overlay25Task *, BattleSceneObject *, Overlay25WorkPrefix *);
static inline void Overlay25Object_GetViewPosition(BattlePosition *pos, BattleSceneObject *object)
{
    BattlePosition_StoreViewRelative(
        pos, object->x, (s16)(object->y - object->z), (s16)(object->effect_anchor_z + 16 * (256 - object->y)),
        object->flags.bits.use_raw_position, object->flags.bits.use_alternate_model);
}
static inline void Overlay25Object_StartDistance(int dx, int dy)
{
    *(vu16 *)0x40002B0 = 0;
    *(vu32 *)0x40002B8 = dx * dx + dy * dy;
    while (*(vu16 *)0x40002B0 & 0x8000) {
    }
}
}
#endif

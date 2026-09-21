/* Cannonballers landing bounces, overlay 12, 0x020C34E0-0x020C3888. */
#include "motion_internal.h"

extern "C" void Overlay12Controller_StartLandingBounce(Overlay12AttackController *controller, int bounce)
{
    BattleSceneObject *object = controller->primary_object;
    BattleSceneObject *secondary = controller->secondary_object;
    BattleEntity_BindResource(object->actor_id,
        controller->controller_flags.bits.mode <= 1 ? 57 : 58);
    BattleEntity_BindResource(secondary->actor_id,
        controller->controller_flags.bits.mode <= 1 ? 57 : 58);
    BattleSceneObject_SetAnimationFromComponent(object,
        controller->flags.bits.formation_variant <= 1 ? 4 : 5, 0, controller->argument);
    BattleSceneObject_SetAnimation(secondary,
        controller->flags.bits.formation_variant <= 1 ? 4 : 5, 1);
    BattleModel *model = BattleSceneObject_GetActiveModel(object);
    model->flags = (model->flags & ~0xF000) | 0x1000;
    model = BattleSceneObject_GetActiveModel(secondary);
    model->flags = (model->flags & ~0xF000) | 0x1000;
    /* Sum three damped arcs to time the simultaneous return home. */
    if (bounce == 0) {
        int duration = 0;
        int height = 200;
        for (int i = 0; i < 3; ++i) {
            int velocity = FX_Sqrt((int)(4096.0 * (double)height)) / 16;
            duration += BattleSceneObject_StartVerticalArc(object, 3, velocity,
                height, i == 0 ? 0 : object->z);
            /* Keep the original double calculation and truncate each step. */
            height = (int)(153.6 * (double)height / 256.0);
            BattleMotionChannel *channel = BattleSceneObject_GetMotionChannel(object, 3);
            channel->callback = 0;
            channel->has_deferred_delta = 0;
        }
        s16 x, y;
        /* The position helper consumes only the shared formation byte at +24. */
        Overlay12Attack_GetHomePosition((Overlay12AttackState *)controller, &x, &y);
        BattleSceneObject_MoveBy(object, 2, x - object->x, y - object->y, 0, duration);
        controller->counter_16 = 0;
        BattlePosition position;
        /* Preserve both halfword truncations in the projected effect height. */
        s16 offset = 12 - object->effect_anchor_z;
        BattlePosition_StoreViewRelative(&position, object->x, (s16)(object->y - object->z),
            (s16)((s16)(object->effect_anchor_z + offset) + 16 * (256 - object->y)),
            object->flags.bits.use_raw_position, object->flags.bits.use_alternate_model);
        BattleModelEffect_Spawn(27, 0, position.x, position.y, position.z, 256);
        BattleSpriteEffect_Spawn(12, position.x, position.y, position.z, 256);
    }
    int height = 200;
    for (int i = 0; i < bounce; ++i)
        height = (int)(153.6 * (double)height / 256.0);
    int velocity = FX_Sqrt((int)(4096.0 * (double)height)) / 16;
    BattleMotionChannel *channel = BattleSceneObject_GetMotionChannel(object, 3);
    channel->callback = 0;
    channel->has_deferred_delta = 0;
    BattleSceneObject_StartVerticalArc(object, 3, velocity, height, 0);
    BattleSound_Play((u16)(bounce == 0 ? 37 : 43), 0, 0, 0);
    controller->flags.bits.state = 12;
}

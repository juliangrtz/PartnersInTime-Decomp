#include <game/battle_object.h>
#include <game/overlay015_attack.h>

enum Overlay15AttackObjectId {
    OVERLAY15_PRIMARY_OBJECT_ID = 40,
    OVERLAY15_SECONDARY_OBJECT_ID = 42,
};

enum Overlay15AttackObjectPairFlag {
    OVERLAY15_OBJECT_PAIR_FLAG_10 = 0x10,
    OVERLAY15_OBJECT_PAIR_FLAG_20 = 0x20,
    OVERLAY15_OBJECT_PAIR_FLAG_80 = 0x80,
};

extern void func_ov002_02071750(BattleSceneObject *object, int value);
extern void func_0200940c(BattleModel *model, int scale);

/* Metrowerks emits C functions in reverse source order. */
void Overlay15Attack_PrepareSecondaryActor(
    Overlay15AttackModelController *state) {
    BattleSceneObject *secondary = state->hit_motion.object;
    BattleSceneObject *primary = state->primary->object;
    u16 hit_actor_id =
        (state->primary->flags & 1) ? 9 : 8;
    BattleHitDescriptor_DisableByActor(hit_actor_id);
    BattleSceneObject_StopMotionChannel(secondary, 3, 0);
    BattleEntity_BindResource(primary->actor_id,
                              state->secondary_resource_id);
    BattleSceneObject_SetAnimation(primary, 42, -1);
    BattleSceneObject_GetActiveModel(primary);
    func_ov002_02071750(primary, 0);
    BattleSceneObject_SetAnimation(secondary, -1, -1);
    Overlay15Attack_ConfigureModelFlags(state);
    state->phase_flags.raw =
        (state->phase_flags.raw & ~0x1F) | 10;
}

void Overlay15Attack_PreparePrimaryActor(
    Overlay15AttackModelController *state) {
    BattleSceneObject *object = state->primary->object;
    BattleModel *model = BattleSceneObject_GetActiveModel(object);

    BattleEntity_BindResource(object->actor_id, state->primary_resource_id);
    BattleSceneObject_SetAnimation(object, 0, -1);
    func_ov002_02071750(object, 1);
    func_0200940c(model, 0x100);
    Overlay15Attack_ConfigureModelFlags(state);
}

void Overlay15Attack_InitializeObjectPair(
    Overlay15AttackObjectPairState *state) {
    state->flags &= ~OVERLAY15_OBJECT_PAIR_FLAG_10;
    state->phase = 0;
    state->timer = 0;
    state->primary = BattleSceneObject_GetById(OVERLAY15_PRIMARY_OBJECT_ID);
    state->secondary = BattleSceneObject_GetById(OVERLAY15_SECONDARY_OBJECT_ID);
    state->flags &= ~OVERLAY15_OBJECT_PAIR_FLAG_20;
    state->rotation = 0;
    state->flags |= OVERLAY15_OBJECT_PAIR_FLAG_80;
    state->primary->effect_anchor_z = -16;
}

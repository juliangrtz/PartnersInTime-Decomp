/*
 * Party recoil: follow the partner shadow while the actor's vertical arc runs,
 * wait between bounces, then reduce the next arc or finish the reaction.
 * The creator copies a 16-byte formation preset into a task from pool 0x8B44.
 * Motion channels 2 and 3 govern travel and the vertical arc respectively.
 */
#include <game/battle_actor.h>
#include <game/battle_scene.h>
#include <game/battle_effect.h>
#include <game/battle_party.h>
#include <nitro/fx.h>

typedef struct BattlePartyReboundPayload {
    union {
        u16 raw;
        struct {
            u16 actor_id : 7;
            u16 object_id : 7;
            u16 unknown : 2;
        } bits;
    } ids;
    s16 initial_height;
    s16 decay_percent;
    s16 gravity;
    u16 delay;
    s16 timer;
    int amplitude;
} BattlePartyReboundPayload;
typedef struct BattlePartyReboundTask {
    struct BattlePartyReboundTask *next;
    void (*callback)(struct BattlePartyReboundTask *);
    struct BattlePartyReboundTask **owner;
    BattlePartyReboundPayload payload;
} BattlePartyReboundTask;
typedef char BattlePartyReboundPayloadSize[
    sizeof(BattlePartyReboundPayload) == 16 ? 1 : -1];
typedef char BattlePartyReboundTaskSize[
    sizeof(BattlePartyReboundTask) == 28 ? 1 : -1];
/* Object-relative model effect; the shared spawn routine returns a task. */
extern BattleAITask *func_ov002_02072004(
    BattleSceneObject *object, const s8 *offset, int animation,
    int depth_side, int resource_variant);

void BattleParty_UpdateReboundShadow(BattleSceneObject *object,
                                    BattleSceneObject *reference) {
    int projected_y;
    object->x = reference->x;
    object->y = reference->y;
    projected_y = reference->y - reference->z;
    if (projected_y >= 0)
        object->z = -192;
    else
        object->z = object->y - (projected_y + 282);
}

void BattleParty_UpdateRebound(BattlePartyReboundTask *task) {
    BattlePartyReboundPayload *state = &task->payload;
    BattlePartyActor *actor = (BattlePartyActor *)
        BattleActor_GetPartySlot(task->payload.ids.bits.actor_id);
    BattleSceneObject *object = actor->actor.scene_object;
    BattleParty_UpdateReboundShadow(
        BattleSceneObject_GetById(task->payload.ids.bits.object_id), object);
    if (!BattleSceneObject_IsAnimationChannelActive(object, 3)) {
        if (!object->time_step_adjustment_q8) {
            int baby = actor->formation_index == 2 || actor->formation_index == 3;
            func_ov002_02072004(object, 0, baby ? 17 : 16, 0, 0);
            BattleSound_Play(57, 0, 0, 0);
            object->time_step_adjustment_q8 = -256;
        }
        if (--state->timer < 0) {
            object->time_step_adjustment_q8 = 0;
            if (!BattleSceneObject_IsAnimationChannelActive(object, 2)) {
                BattleSceneObject_SetAnimation(object, 0, -1);
                BattleSceneObject_SetAnimation(
                    BattleSceneObject_GetById(state->ids.bits.object_id), -1, 1);
                object->time_step_adjustment_q8 = 0;
                actor->actor.flags &= ~0x200;
                if (!BattleActor_CanReceiveStatus(BattleActor_GetById(object->actor_id)))
                    BattleParty_StartKnockout(object);
                actor->actor.flags &= ~0x8000;
                task->callback = 0;
            } else {
                int amplitude = state->amplitude -
                    state->amplitude * state->decay_percent / 100;
                int velocity = FX_Sqrt(amplitude * (-32 * state->gravity));
                BattleSceneObject_StartVerticalArc(object, 3, velocity / 16,
                                                   amplitude, 0);
                BattleSound_Play(43, 0, 0, 0);
                state->timer = state->delay;
                state->amplitude = amplitude;
            }
        }
    }
}

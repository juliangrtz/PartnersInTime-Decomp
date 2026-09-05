#include <game/battle_actor.h>
#include <game/overlay026_attack.h>

enum Overlay26PartyPairingConstant {
    OVERLAY26_FIRST_BABY_ACTOR_ID = BATTLE_ACTOR_BABY_MARIO,
    OVERLAY26_FIRST_BABY_FORMATION = BATTLE_FORMATION_BABY_MARIO,
    OVERLAY26_ADULT_ACTOR_ID_OFFSET = 2,
};

/* Metrowerks emits C functions in reverse source order. */
int Overlay26Attack_IsValidPartnerTarget(BattlePartyActor *actor) {
    BattlePartyActor *target = (BattlePartyActor *)BattleActor_GetPartySlot(
        (u16)actor->actor.target_actor_id);

    if ((target->formation_index & 1) ==
            (actor->formation_index & 1) &&
        Overlay26Attack_IsBabyPairReady(target->actor.scene_object)) {
        return 1;
    }
    return (u16)actor->actor.target_actor_id ==
           actor->actor.scene_object->actor_id;
}

int Overlay26Attack_IsBabyPairReady(BattleSceneObject *object) {
    int result = 0;
    int adult_ready = 0;

    if ((object->actor_id & ~1) == OVERLAY26_FIRST_BABY_ACTOR_ID) {
        BattleActor *adult = BattleActor_GetById(
            (u16)(object->actor_id -
                  OVERLAY26_ADULT_ACTOR_ID_OFFSET));

        if (BattleActor_CanReceiveStatus(adult)) {
            adult_ready = 1;
        }
    }
    if (adult_ready &&
        ((((BattlePartyActor *)BattleActor_GetPartySlot(object->actor_id))
              ->formation_index & ~1) ==
         OVERLAY26_FIRST_BABY_FORMATION)) {
        result = 1;
    }
    return result;
}

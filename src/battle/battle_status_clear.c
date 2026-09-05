#include <game/battle_scene.h>
#include <game/battle_status.h>

void BattleStatus_ClearAll(BattleActor *actor) {
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_AILMENT_1);
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_AILMENT_2);
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_AILMENT_3);
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_AILMENT_4);
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_AILMENT_5);
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_POWER_CHANGE);
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_DEFENSE_CHANGE);
    BattleStatus_ClearEffect(actor, BATTLE_STATUS_SPEED_CHANGE);
}

int BattleStatus_ClearEffect(BattleActor *actor, int status_id) {
    BattleStatusState *state;

    switch (status_id) {
    case BATTLE_STATUS_AILMENT_1:
        state = &actor->ailment_group_1;
        if (state->kind != 1) {
            return BATTLE_STATUS_NONE;
        }
        break;
    case BATTLE_STATUS_AILMENT_2:
        state = &actor->ailment_group_1;
        if (state->kind != 2) {
            return BATTLE_STATUS_NONE;
        }
        break;
    case BATTLE_STATUS_AILMENT_3:
        state = &actor->ailment_group_1;
        if (state->kind != 3) {
            return BATTLE_STATUS_NONE;
        }
        break;
    case BATTLE_STATUS_AILMENT_4:
        state = &actor->ailment_group_2;
        if (state->kind != 1) {
            return BATTLE_STATUS_NONE;
        }
        break;
    case BATTLE_STATUS_AILMENT_5:
        state = &actor->ailment_group_2;
        if (state->kind != 2) {
            return BATTLE_STATUS_NONE;
        }
        break;
    case BATTLE_STATUS_POWER_CHANGE:
        state = &actor->power_change;
        if (state->kind == 0) {
            return BATTLE_STATUS_NONE;
        }
        break;
    case BATTLE_STATUS_DEFENSE_CHANGE:
        state = &actor->defense_change;
        if (state->kind == 0) {
            return BATTLE_STATUS_NONE;
        }
        break;
    case BATTLE_STATUS_SPEED_CHANGE:
        state = &actor->speed_change;
        if (state->kind == 0) {
            return BATTLE_STATUS_NONE;
        }
        break;
    default:
        return BATTLE_STATUS_NONE;
    }

    BattleStatus_StopActorEffect(actor->scene_object->actor_id, status_id);
    state->kind = 0;

    switch (status_id) {
    case BATTLE_STATUS_POWER_CHANGE:
        actor->power = actor->base_power;
        break;
    case BATTLE_STATUS_DEFENSE_CHANGE:
        actor->defense = actor->base_defense;
        break;
    case BATTLE_STATUS_SPEED_CHANGE:
        actor->speed = actor->base_speed;
        break;
    }

    return status_id;
}

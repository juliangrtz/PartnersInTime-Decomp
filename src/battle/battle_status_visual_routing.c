#include <game/battle_actor.h>
#include <game/battle_status.h>

void BattleStatus_SpawnEffect(int actor_id, int status_id) {
    BattleActor *actor = BattleActor_GetById(actor_id);

    if ((unsigned int)actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
        (unsigned int)actor_id <
            BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
        switch (status_id) {
        case BATTLE_STATUS_AILMENT_1: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 1) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_2: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 2) {
                status->effect =
                    BattleStatus_StartEnemyAilment2Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_3: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 3) {
                status->effect =
                    BattleStatus_StartEnemyAilment3Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_4: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 1) {
                status->effect =
                    BattleStatus_StartEnemyAilment4Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_5: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 2) {
                status->effect =
                    BattleStatus_StartEnemyAilment5Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_POWER_CHANGE:
            actor->power_change.effect =
                BattleStatus_StartEnemyStatVisual(actor_id, status_id);
            return;
        case BATTLE_STATUS_DEFENSE_CHANGE:
            actor->defense_change.effect =
                BattleStatus_StartEnemyStatVisual(actor_id, status_id);
            return;
        case BATTLE_STATUS_SPEED_CHANGE:
            actor->speed_change.effect =
                BattleStatus_StartEnemyStatVisual(actor_id, status_id);
            return;
        default:
            break;
        }
    } else {
        switch (status_id) {
        case BATTLE_STATUS_AILMENT_1: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 1) {
                status->effect =
                    BattleStatus_StartPartyAilment1Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_2: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 2) {
                status->effect =
                    BattleStatus_StartPartyAilment2Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_3: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 3) {
                status->effect =
                    BattleStatus_StartPartyAilment3Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_4: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 1) {
                status->effect =
                    BattleStatus_StartPartyAilment4Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_5: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 2) {
                status->effect =
                    BattleStatus_StartPartyAilment5Visual(actor_id);
            }
            return;
        }
        case BATTLE_STATUS_POWER_CHANGE:
            actor->power_change.effect =
                BattleStatus_StartPartyStatVisual(actor_id, status_id);
            return;
        case BATTLE_STATUS_DEFENSE_CHANGE:
            actor->defense_change.effect =
                BattleStatus_StartPartyStatVisual(actor_id, status_id);
            return;
        case BATTLE_STATUS_SPEED_CHANGE:
            actor->speed_change.effect =
                BattleStatus_StartPartyStatVisual(actor_id, status_id);
            return;
        default:
            break;
        }
    }
}

void BattleStatus_StopActorEffect(int actor_id, int status_id) {
    BattleActor *actor = BattleActor_GetById(actor_id);

    if ((unsigned int)actor_id >= BATTLE_ACTOR_ENEMY_FIRST &&
        (unsigned int)actor_id <
            BATTLE_ACTOR_ENEMY_FIRST + BATTLE_ACTOR_ENEMY_COUNT) {
        switch (status_id) {
        case BATTLE_STATUS_AILMENT_1: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 1) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_2: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 2) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_3: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 3) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_4: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 1) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_5: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 2) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_POWER_CHANGE:
            actor->power_change.effect = 0;
            return;
        case BATTLE_STATUS_DEFENSE_CHANGE:
            actor->defense_change.effect = 0;
            return;
        case BATTLE_STATUS_SPEED_CHANGE:
            actor->speed_change.effect = 0;
            return;
        default:
            break;
        }
    } else {
        switch (status_id) {
        case BATTLE_STATUS_AILMENT_1: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 1) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_2: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 2) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_3: {
            BattleStatusState *status = &actor->ailment_group_1;
            if (status->kind == 3) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_4: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 1) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_AILMENT_5: {
            BattleStatusState *status = &actor->ailment_group_2;
            if (status->kind == 2) {
                status->effect = 0;
            }
            return;
        }
        case BATTLE_STATUS_POWER_CHANGE:
            actor->power_change.effect = 0;
            return;
        case BATTLE_STATUS_DEFENSE_CHANGE:
            actor->defense_change.effect = 0;
            return;
        case BATTLE_STATUS_SPEED_CHANGE:
            actor->speed_change.effect = 0;
            return;
        default:
            break;
        }
    }
}

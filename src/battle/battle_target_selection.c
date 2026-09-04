#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_effect.h>
#include <game/battle_interface_assets.h>
#include <game/battle_scene.h>
#include <game/item.h>

enum BattleTargetSelectionConstant {
    BATTLE_ACTIVE_ACTOR_ID_OFFSET = 0x20,
    BATTLE_INPUT_PRESSED_OFFSET = 0x104,
    BATTLE_TARGET_MODE_OFFSET = 0x112,
    BATTLE_TARGET_ACTOR_ID_OFFSET = 0x118,
    BATTLE_SELECTED_ACTION_ITEM_OFFSET = 0x130,
    BATTLE_SELECTED_ACTION_ITEM_SCROLL_OFFSET = 0x11E,
    BATTLE_SELECTED_ACTION_ITEM_INDEX_OFFSET = 0x120,
    BATTLE_SELECTED_USABLE_ITEM_SCROLL_OFFSET = 0x122,
    BATTLE_SELECTED_USABLE_ITEM_INDEX_OFFSET = 0x124,
    BATTLE_COMMAND_WHEEL_OBJECT_OFFSET = 0x6530,
    BATTLE_TARGET_FOCUS_OBJECT_OFFSET = 0x6568,
    BATTLE_TARGET_INTERFACE_FLAGS_OFFSET = 0x6574,
    BATTLE_ITEM_CURSOR_ANCHOR_OFFSET = 0x65E8,
    BATTLE_COMMAND_MODE_OFFSET = 0x65EE,
    BATTLE_TARGET_CURSOR_VISIBLE_OFFSET = 0x673A,
    BATTLE_INPUT_RIGHT = 0x10,
    BATTLE_INPUT_LEFT = 0x20,
    BATTLE_INPUT_UP = 0x40,
    BATTLE_INPUT_DOWN = 0x80,
    BATTLE_TARGET_FLAG_UNIFORM = 0x100,
    BATTLE_TARGET_ALL = -1,
    BATTLE_VISIBLE_ENEMY_COUNT = 6
};

enum BattleTargetSelectionMode {
    BATTLE_TARGET_MODE_ENEMY_1 = 1,
    BATTLE_TARGET_MODE_ENEMY_2 = 2,
    BATTLE_TARGET_MODE_ACTION_ITEM = 3
};

extern void BattleTargetCursor_MoveToActor(int duration, int actor_id,
                                           int animation_id);
extern void BattleTargetCursor_MoveToGroup(int duration, int x, int y,
                                           int animation_id);

void BattleItemCursor_MoveToSelection(int duration) {
    u8 *context = gBattleContext;

    if (*(s16 *)(context + BATTLE_COMMAND_MODE_OFFSET) != 0) {
        BattleSceneObject *anchor =
            *(BattleSceneObject **)(context + BATTLE_ITEM_CURSOR_ANCHOR_OFFSET);
        int target_y;
        int target_x;
        int row_index = -1;

        target_y = anchor->y - anchor->z + 8;
        target_x = anchor->x - 62;
        if (*(s16 *)(context + BATTLE_TARGET_MODE_OFFSET) == 3) {
            row_index =
                *(s16 *)(context + BATTLE_SELECTED_ACTION_ITEM_INDEX_OFFSET) -
                *(s16 *)(context + BATTLE_SELECTED_ACTION_ITEM_SCROLL_OFFSET);
        }
        if (*(s16 *)(context + BATTLE_TARGET_MODE_OFFSET) == 4) {
            row_index = *(s16 *)(gBattleContext +
                                 BATTLE_SELECTED_USABLE_ITEM_INDEX_OFFSET) -
                        *(s16 *)(gBattleContext +
                                 BATTLE_SELECTED_USABLE_ITEM_SCROLL_OFFSET);
        }
        if (row_index < 0) {
            return;
        }
        if (row_index >= 5) {
            return;
        }
        BattleTargetCursor_MoveToGroup(duration, target_x,
                                       target_y + 15 * row_index, 0);
        return;
    }

    {
        BattleSceneObject *anchor = *(
            BattleSceneObject **)(context + BATTLE_COMMAND_WHEEL_OBJECT_OFFSET);

        BattleTargetCursor_MoveToGroup(duration, anchor->x - 4,
                                       anchor->y - anchor->z + 32, 20);
    }
}

int BattleTarget_ComputeDirectionalScore(int direction_x, int direction_y,
                                         int candidate_actor_id);

int BattleTargetSelection_UpdateEnemyTarget(int initialize) {
    BattlePartyActor *active_actor =
        (BattlePartyActor *)BattleActor_GetPartySlot(
            *(u16 *)(gBattleContext + BATTLE_ACTIVE_ACTOR_ID_OFFSET));

    if (initialize != 0) {
        BattleActor *target = BattleActor_GetById(
            *(u16 *)(gBattleContext + BATTLE_TARGET_ACTOR_ID_OFFSET));

        if (!BattleActor_CanReceiveStatus(target) &&
            (*(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                 BATTLE_TARGET_MODE_ENEMY_1 ||
             *(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                 BATTLE_TARGET_MODE_ENEMY_2 ||
             *(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                 BATTLE_TARGET_MODE_ACTION_ITEM)) {
            int enemy_index;

            for (enemy_index = 0;; enemy_index++) {
                target = BattleActor_GetById(
                    (u16)(enemy_index + BATTLE_ACTOR_ENEMY_FIRST));
                if (BattleActor_CanReceiveStatus(target)) {
                    if (!BattleActor_GetById(
                             (u16)(enemy_index + BATTLE_ACTOR_ENEMY_FIRST))
                             ->flag_bits.excluded_from_targeting) {
                        break;
                    }
                }
            }
            *(u16 *)(gBattleContext + BATTLE_TARGET_ACTOR_ID_OFFSET) =
                enemy_index + BATTLE_ACTOR_ENEMY_FIRST;
        }

        *(u16 *)(gBattleContext + BATTLE_TARGET_CURSOR_VISIBLE_OFFSET) = 1;
        {
            int cursor_animation = 8;

            if ((u16)(active_actor->formation_index - 4) <= 1 &&
                *(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                    BATTLE_TARGET_MODE_ENEMY_2) {
                cursor_animation = 10;
            }
            BattleTargetCursor_MoveToActor(
                8, *(u16 *)(gBattleContext + BATTLE_TARGET_ACTOR_ID_OFFSET),
                cursor_animation);
        }

        if (*(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                BATTLE_TARGET_MODE_ACTION_ITEM &&
            (*(ActionItemRecord **)(gBattleContext +
                                    BATTLE_SELECTED_ACTION_ITEM_OFFSET))
                ->target_flags.bits.allows_uniform_targeting) {
            return BattleInterface_LoadTargetLabelResource(20);
        }
        return BattleInterface_LoadTargetName(
            *(u16 *)(gBattleContext + BATTLE_TARGET_ACTOR_ID_OFFSET));
    }

    {
        int direction_x = 0;
        u8 *context = gBattleContext;
        int direction_y = 0;
        s16 input = *(u16 *)(context + BATTLE_INPUT_PRESSED_OFFSET);
        int best_score = 0;

        if (input & BATTLE_INPUT_LEFT) {
            direction_x = -8;
        }
        if (input & BATTLE_INPUT_RIGHT) {
            direction_x = 8;
        }
        if (input & BATTLE_INPUT_UP) {
            direction_y = -8;
        }
        if (input & BATTLE_INPUT_DOWN) {
            direction_y = 8;
        }

        {
            int next_target_id = 0;

            if (*(s16 *)(context + BATTLE_TARGET_MODE_OFFSET) ==
                    BATTLE_TARGET_MODE_ENEMY_1 ||
                *(s16 *)(context + BATTLE_TARGET_MODE_OFFSET) ==
                    BATTLE_TARGET_MODE_ENEMY_2 ||
                (*(s16 *)(context + BATTLE_TARGET_MODE_OFFSET) ==
                     BATTLE_TARGET_MODE_ACTION_ITEM &&
                 !(*(ActionItemRecord **)(context +
                                          BATTLE_SELECTED_ACTION_ITEM_OFFSET))
                      ->target_flags.bits.allows_uniform_targeting)) {
                if ((*(u16 *)(context + BATTLE_TARGET_INTERFACE_FLAGS_OFFSET) &
                     BATTLE_TARGET_FLAG_UNIFORM) == 0) {
                    int enemy_index;

                    for (enemy_index = 0;
                         enemy_index < BATTLE_VISIBLE_ENEMY_COUNT;
                         enemy_index++) {
                        u16 actor_id = enemy_index + BATTLE_ACTOR_ENEMY_FIRST;
                        BattleActor *enemy = BattleActor_GetEnemySlot(actor_id);

                        if (enemy->resource_slot != 0 &&
                            enemy->current_hp > 0 &&
                            !enemy->flag_bits.excluded_from_targeting) {
                            int score = BattleTarget_ComputeDirectionalScore(
                                direction_x, direction_y, actor_id);

                            if (best_score < score) {
                                best_score = score;
                                next_target_id = actor_id;
                            }
                        }
                    }

                    if ((input & BATTLE_INPUT_LEFT) &&
                        (*(u16 *)(context +
                                  BATTLE_TARGET_INTERFACE_FLAGS_OFFSET) &
                         BATTLE_TARGET_FLAG_UNIFORM) == 0 &&
                        next_target_id == 0) {
                        next_target_id = BATTLE_TARGET_ALL;
                    }
                } else if ((input & BATTLE_INPUT_RIGHT) &&
                           !active_actor->state_flags.bits
                                .target_selection_locked) {
                    BattleSound_Play(1, 0, 0, 0);
                    next_target_id =
                        *(s16 *)(context + BATTLE_TARGET_ACTOR_ID_OFFSET);
                }
            } else if (*(s16 *)(context + BATTLE_TARGET_MODE_OFFSET) ==
                           BATTLE_TARGET_MODE_ACTION_ITEM &&
                       (*(ActionItemRecord *
                              *)(context + BATTLE_SELECTED_ACTION_ITEM_OFFSET))
                           ->target_flags.bits.allows_uniform_targeting) {
                if ((*(u16 *)(context + BATTLE_TARGET_INTERFACE_FLAGS_OFFSET) &
                     BATTLE_TARGET_FLAG_UNIFORM) == 0) {
                    if ((input & BATTLE_INPUT_LEFT) &&
                        !active_actor->state_flags.bits
                             .target_selection_locked) {
                        next_target_id = BATTLE_TARGET_ALL;
                    }
                } else if ((input & BATTLE_INPUT_RIGHT) &&
                           !active_actor->state_flags.bits
                                .target_selection_locked) {
                    BattleSound_Play(1, 0, 0, 0);
                    next_target_id =
                        *(s16 *)(context + BATTLE_TARGET_ACTOR_ID_OFFSET);
                }
            }

            if (next_target_id != 0 &&
                *(s16 *)(context + BATTLE_TARGET_ACTOR_ID_OFFSET) !=
                    next_target_id) {
                BattleSound_Play(1, 0, 0, 0);
            }

            if (next_target_id == BATTLE_TARGET_ALL) {
                BattleSceneObject *focus =
                    *(BattleSceneObject **)(context +
                                            BATTLE_TARGET_FOCUS_OBJECT_OFFSET);
                int cursor_animation = 20;

                *(u16 *)(gBattleContext +
                         BATTLE_TARGET_INTERFACE_FLAGS_OFFSET) |=
                    BATTLE_TARGET_FLAG_UNIFORM;
                if ((u16)(active_actor->formation_index - 4) <= 1 &&
                    *(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                        BATTLE_TARGET_MODE_ENEMY_2) {
                    cursor_animation = 22;
                }
                BattleTargetCursor_MoveToGroup(6, focus->x - 4,
                                               focus->y - focus->z + 32,
                                               cursor_animation);
                BattleInterface_LoadTargetLabelResource(8);
            } else if (next_target_id != 0) {
                int cursor_animation = 8;

                *(u16 *)(gBattleContext + BATTLE_TARGET_ACTOR_ID_OFFSET) =
                    next_target_id;
                *(u16 *)(gBattleContext +
                         BATTLE_TARGET_INTERFACE_FLAGS_OFFSET) &=
                    ~BATTLE_TARGET_FLAG_UNIFORM;
                if ((u16)(active_actor->formation_index - 4) <= 1 &&
                    *(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                        BATTLE_TARGET_MODE_ENEMY_2) {
                    cursor_animation = 10;
                }
                BattleTargetCursor_MoveToActor(6, (u16)next_target_id,
                                               cursor_animation);
                if (*(s16 *)(gBattleContext + BATTLE_TARGET_MODE_OFFSET) ==
                        BATTLE_TARGET_MODE_ACTION_ITEM &&
                    (*(ActionItemRecord **)(gBattleContext +
                                            BATTLE_SELECTED_ACTION_ITEM_OFFSET))
                        ->target_flags.bits.allows_uniform_targeting) {
                    BattleInterface_LoadTargetLabelResource(20);
                } else {
                    BattleInterface_LoadTargetName((u16)next_target_id);
                }
            }

            active_actor->state_flags.bits.uniform_target_selection =
                (*(u16 *)(gBattleContext +
                          BATTLE_TARGET_INTERFACE_FLAGS_OFFSET) &
                 BATTLE_TARGET_FLAG_UNIFORM) != 0;
            return active_actor->state_flags.raw;
        }
    }
}

int BattleTarget_ComputeDirectionalScore(int direction_x, int direction_y,
                                         int candidate_actor_id) {
    BattleSceneObject *candidate;
    BattleSceneObject *current;
    int candidate_x;
    int candidate_y;
    int current_x;
    int current_y;
    int delta_y;
    int delta_x;
    int dot_product;
    int cross_product;

    if (direction_x == 0 && direction_y == 0) {
        return 0;
    }

    candidate = BattleSceneObject_GetById(candidate_actor_id);
    candidate_x = candidate->x + candidate->property_0fc;
    candidate_y = (candidate->y - candidate->z) +
                  (candidate->property_0fd - candidate->property_0f8);
    current = BattleSceneObject_GetById(
        *(u16 *)(gBattleContext + BATTLE_TARGET_ACTOR_ID_OFFSET));
    current_y = (current->y - current->z) +
                (current->property_0fd - current->property_0f8);
    current_x = current->x + current->property_0fc;
    delta_x = candidate_x - current_x;
    delta_y = candidate_y - current_y;
    dot_product = direction_y * delta_y;
    dot_product += direction_x * delta_x;
    if (dot_product > 0) {
        cross_product = direction_y * delta_x - direction_x * delta_y;
        return 0x40000000 / (dot_product + cross_product * cross_product);
    }
    return 0;
}

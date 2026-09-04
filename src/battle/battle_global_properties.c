#include <game/battle_actor.h>
#include <game/battle_context.h>
#include <game/battle_global_properties.h>
#include <game/save_data.h>

int BattleGlobalProperty_Get(int property) {
    switch (property) {
    case BATTLE_GLOBAL_PROPERTY_00:
        return *(int *)(gBattleContext + BATTLE_GLOBAL_00_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_01:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_01_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_02:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_02_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_03:
        return *(u16 *)(gBattleContext + BATTLE_GLOBAL_03_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_04:
        return *(u16 *)(gBattleContext + BATTLE_GLOBAL_04_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_SAVE_558:
        return *(s16 *)(gSaveData + SAVE_GLOBAL_05_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_ACTIVE_PARTY_ACTOR:
        return *(u16 *)(gBattleContext +
                        BATTLE_GLOBAL_ACTIVE_PARTY_ACTOR_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_ACTIVE_TARGET_ACTOR:
        return (u16)BattleActor_GetPartySlot(
            *(u16 *)(gBattleContext +
                     BATTLE_GLOBAL_ACTIVE_PARTY_ACTOR_OFFSET))
            ->target_actor_id;
    case BATTLE_GLOBAL_PROPERTY_09:
        return *(u16 *)(gBattleContext + BATTLE_GLOBAL_09_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_10:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_10_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_MAP_ID:
        return *(s16 *)(gSaveData + SAVE_MAP_ID_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_12:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_13:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_13_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_01:
        return ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_14;
    case BATTLE_GLOBAL_PROPERTY_15:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_15_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_16:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_16_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_CONSTANT_NEGATIVE_ONE:
        return -1;
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_10:
        return ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_19;
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_12:
        return ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_20;
    case BATTLE_GLOBAL_PROPERTY_BACKGROUND_ID:
        return ((BattleContext *)gBattleContext)->background_id;
    case BATTLE_GLOBAL_PROPERTY_FADE_LEVEL:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_FADE_LEVEL_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_FADE_COMPLETE:
        return *(s16 *)(gBattleContext +
                        BATTLE_GLOBAL_FADE_COUNTER_OFFSET) >= 31;
    case BATTLE_GLOBAL_PROPERTY_24:
        return *(s16 *)(gBattleContext + BATTLE_GLOBAL_24_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_INVERTED_MASK:
        return ~*(u16 *)(gBattleContext + BATTLE_GLOBAL_MASK_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_MASK:
        return *(u16 *)(gBattleContext + BATTLE_GLOBAL_MASK_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_27:
        return *(u16 *)(gBattleContext + BATTLE_GLOBAL_27_OFFSET);
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_17:
        return ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.alternate_formation;
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_02:
        return ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_29;
    case BATTLE_GLOBAL_PROPERTY_CONSTANT_ZERO:
        return 0;
    default:
        return 0;
    }
}

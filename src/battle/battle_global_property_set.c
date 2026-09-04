#include <game/battle_actor.h>
#include <game/battle_background.h>
#include <game/battle_context.h>
#include <game/battle_global_properties.h>
#include <game/battle_hit.h>
#include <game/save_data.h>

typedef struct BattlePartyGlobalPropertyView {
    BattleActor actor;
    u8 unknown_070[4];
    u16 hit_flags;
} BattlePartyGlobalPropertyView;

void SceneBackground_StartFade(int fade_out);

void BattleGlobalProperty_Set(int property, int value) {
    switch (property) {
    case BATTLE_GLOBAL_PROPERTY_00:
        *(int *)(gBattleContext + BATTLE_GLOBAL_00_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_01:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_01_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_02:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_02_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_03:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_03_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_04:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_04_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_SAVE_558:
        *(u16 *)(gSaveData + SAVE_GLOBAL_05_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_ACTIVE_PARTY_ACTOR:
        *(u16 *)(gBattleContext +
                 BATTLE_GLOBAL_ACTIVE_PARTY_ACTOR_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_ACTIVE_TARGET_ACTOR:
        BattleActor_GetPartySlot(
            *(u16 *)(gBattleContext +
                     BATTLE_GLOBAL_ACTIVE_PARTY_ACTOR_OFFSET))
            ->target_actor_id = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_09:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_09_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_10:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_10_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_MAP_ID:
        *(u16 *)(gSaveData + SAVE_MAP_ID_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_12:
        if (*(s16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET) != value) {
            *(u16 *)(gBattleContext + BATTLE_GLOBAL_12_OFFSET) = value;
            BattleHitDescriptor_DisableByActor(56);
            BattleHitDescriptor_DisableByActor(57);
            BattleHitDescriptor_DisableByActor(8);
            BattleHitDescriptor_DisableByActor(9);
            ((BattlePartyGlobalPropertyView *)BattleActor_GetPartySlot(56))
                ->hit_flags &= ~0xF00;
            ((BattlePartyGlobalPropertyView *)BattleActor_GetPartySlot(57))
                ->hit_flags &= ~0xF00;
        }
        break;
    case BATTLE_GLOBAL_PROPERTY_13:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_13_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_01:
        ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_14 = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_15:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_15_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_16:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_16_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_10:
        ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_19 = value != 0;
        break;
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_12:
        ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_20 = value != 0;
        break;
    case BATTLE_GLOBAL_PROPERTY_BACKGROUND_ID:
        ((BattleContext *)gBattleContext)->background_id = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_FADE_LEVEL:
        if (value <= 0) {
            value = 0;
        }
        if (value > 31) {
            value = 31;
        }
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_FADE_LEVEL_OFFSET) = value;
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_FADE_STEP_OFFSET) =
            (32 * (31 - value)) / 31;
        break;
    case BATTLE_GLOBAL_PROPERTY_FADE_COMPLETE:
        ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_23 = value != 0;
        break;
    case BATTLE_GLOBAL_PROPERTY_24:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_24_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_INVERTED_MASK:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_MASK_OFFSET) &= ~value;
        break;
    case BATTLE_GLOBAL_PROPERTY_MASK:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_MASK_OFFSET) |= value;
        break;
    case BATTLE_GLOBAL_PROPERTY_27:
        *(u16 *)(gBattleContext + BATTLE_GLOBAL_27_OFFSET) = value;
        break;
    case BATTLE_GLOBAL_PROPERTY_RUNTIME_FLAG_17:
        ((BattleContext *)gBattleContext)
            ->runtime.flags.bits.global_property_28 = value != 0;
        break;
    case BATTLE_GLOBAL_PROPERTY_RELOAD_BACKGROUND:
        ((BattleContext *)gBattleContext)->background_id = value;
        BattleBackground_RequestLoad();
        break;
    case BATTLE_GLOBAL_PROPERTY_TOGGLE_BACKGROUND:
        BattleBackground_RequestToggle();
        break;
    case BATTLE_GLOBAL_PROPERTY_TOGGLE_FADE: {
        u32 flags = ((BattleContext *)gBattleContext)->runtime.flags.raw;
        u32 fade_out = ((flags << 11) >> 31) ^ 1;

        flags &= ~BATTLE_RUNTIME_FLAG_BACKGROUND_FADED_OUT;
        flags |= (fade_out & 1) << 20;
        ((BattleContext *)gBattleContext)->runtime.flags.raw = flags;
        SceneBackground_StartFade(
            ((BattleContext *)gBattleContext)
                ->runtime.flags.bits.background_faded_out);
        break;
    }
    case BATTLE_GLOBAL_PROPERTY_CONSTANT_ZERO:
        break;
    default:
        break;
    }
}

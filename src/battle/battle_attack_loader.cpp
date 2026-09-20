/*
 * Attack overlay loading (overlay 2, 0x0206F56C-0x0206F948).
 *
 * Starting the selected command or an item: the attack's overlay is requested,
 * and the load is polled each frame until the overlay's entry point can be
 * called.
 */

#include <game/battle_attack_loader.h>
#include <game/overlay015_attack.h>
#include <game/overlay016_attack.h>
#include <game/linker_overlay_ids.h>
extern "C" {
extern void *gBattleSystem;
void ArchiveIO_UnloadOverlay(void *, int);
int ArchiveIO_FinishOverlay(void *);
void BattleSpecialHandle_QueueReload(void *);
void Overlay20Attack_Create(BattlePartyActor *);
void Overlay21Attack_Create(BattlePartyActor *);
void Overlay11Attack_StartRedShell(BattlePartyActor *);
void Overlay11Attack_StartGreenShell(BattlePartyActor *);
void Overlay13Attack_Initialize(BattlePartyActor *);
void func_ov014_020c60f4(BattlePartyActor *);
void func_ov014_020c60e4(BattlePartyActor *);
void Overlay12Attack_Initialize(BattlePartyActor *);
void func_ov018_020c6d68(BattlePartyActor *);
void func_ov017_020c68ec(BattlePartyActor *);
void Overlay26Attack_Initialize(BattlePartyActor *);

void BattleAttack_StartSelectedCommand(BattlePartyActor *actor)
{
    switch (actor->command_id) {
    case 0:
        BattleAttack_RequestOverlay(actor, OVERLAY_20_ID, Overlay20Attack_Create);
        break;
    case 1:
        BattleAttack_RequestOverlay(actor, OVERLAY_21_ID, Overlay21Attack_Create);
        break;
    case 2:
        BattleAttack_RequestOverlay(actor, OVERLAY_11_ID, Overlay11Attack_StartRedShell);
        break;
    case 4:
        BattleAttack_RequestOverlay(actor, OVERLAY_13_ID, Overlay13Attack_Initialize);
        break;
    case 6:
        BattleAttack_RequestOverlay(actor, OVERLAY_11_ID, Overlay11Attack_StartGreenShell);
        break;
    case 7:
        BattleAttack_RequestOverlay(actor, OVERLAY_14_ID, func_ov014_020c60f4);
        break;
    case 8:
        BattleAttack_RequestOverlay(actor, OVERLAY_15_ID, Overlay15Attack_Initialize);
        break;
    case 9:
        BattleAttack_RequestOverlay(actor, OVERLAY_16_ID, Overlay16Attack_Initialize);
        break;
    case 10:
        BattleAttack_RequestOverlay(actor, OVERLAY_12_ID, Overlay12Attack_Initialize);
        break;
    case 11:
        BattleAttack_RequestOverlay(actor, OVERLAY_14_ID, func_ov014_020c60e4);
        break;
    case 12:
        BattleAttack_RequestOverlay(actor, OVERLAY_18_ID, func_ov018_020c6d68);
        break;
    case 13:
        BattleAttack_RequestOverlay(actor, OVERLAY_17_ID, func_ov017_020c68ec);
        break;
    default:
        BattleAttack_RequestOverlay(actor, OVERLAY_11_ID, Overlay11Attack_StartGreenShell);
        break;
    }
}

void BattleAttack_StartItemUse(BattlePartyActor *actor)
{
    BattleAttack_RequestOverlay(actor, OVERLAY_26_ID, Overlay26Attack_Initialize);
}

void BattleAttack_RequestOverlay(BattlePartyActor *actor, void *overlay, BattleAttackCallback callback)
{
    actor->attack_callback = BattleAttack_UpdateOverlayLoad;
    data_ov002_020c0660->requested_overlay = overlay;
    data_ov002_020c0660->phase = BATTLE_ATTACK_LOAD_BEGIN;
    data_ov002_020c0660->callback = callback;
}

void BattleAttack_UpdateOverlayLoad(BattlePartyActor *actor)
{
    u8 *phase = &data_ov002_020c0660->phase;
    switch (*phase) {
    case BATTLE_ATTACK_LOAD_BEGIN:
        if (data_ov002_020c0660->active_overlay)
            ArchiveIO_UnloadOverlay(data_ov002_020c0660->active_overlay, 0);
        data_ov002_020c0660->active_overlay = data_ov002_020c0660->requested_overlay;
        BattleSpecialHandle_QueueReload(&data_ov002_020c0660->requested_overlay);
        data_ov002_020c0660->phase = BATTLE_ATTACK_LOAD_WAIT_REQUEST;
        break;
    case BATTLE_ATTACK_LOAD_WAIT_REQUEST:
        if (!data_ov002_020c0660->requested_overlay)
            *phase = BATTLE_ATTACK_LOAD_WAIT_OVERLAY;
        break;
    case BATTLE_ATTACK_LOAD_WAIT_OVERLAY:
        if (!ArchiveIO_FinishOverlay(gBattleSystem)) {
            if (data_ov002_020c0660->common_requested) {
                data_ov002_020c0660->phase = BATTLE_ATTACK_LOAD_READY;
            } else {
                data_ov002_020c0660->requested_overlay = OVERLAY_10_ID;
                BattleSpecialHandle_QueueReload(&data_ov002_020c0660->requested_overlay);
                data_ov002_020c0660->common_requested = 1;
                data_ov002_020c0660->phase = BATTLE_ATTACK_LOAD_WAIT_COMMON_REQUEST;
            }
        }
        break;
    case BATTLE_ATTACK_LOAD_WAIT_COMMON_REQUEST:
        if (!data_ov002_020c0660->requested_overlay)
            *phase = BATTLE_ATTACK_LOAD_WAIT_COMMON_OVERLAY;
        break;
    case BATTLE_ATTACK_LOAD_WAIT_COMMON_OVERLAY:
        if (!ArchiveIO_FinishOverlay(gBattleSystem))
            actor->attack_callback = data_ov002_020c0660->callback;
        break;
    case BATTLE_ATTACK_LOAD_READY:
        actor->attack_callback = data_ov002_020c0660->callback;
        break;
    }
}
}

/*
 * Field scene dispatch (overlay 0, 0x02069EB0-0x0206A130).
 *
 * Steps the pending transition out of the field into another scene.
 */

#include "field_scene_transition_internal.h"
int FieldSystem_UpdateSceneTransition(FieldSystem *system) {
    switch (system->scene_transition.phase) {
    case 1:
        if (!func_ov000_0206b3a0(system)) {
            GameAudio_StopEffects();
            system->scene_transition.phase = 2;
            FreezeFieldIrq(system);
        }
        break;
    case 2:
        FieldSystem_PrepareBattleScene(system);
        system->task.status = 3;
        system->scene_transition.phase = 0;
        return 1;
    case 3:
        if (!FieldSystem_IsPauseFadeActive(system)) {
            system->scene_transition.phase = 4;
            FreezeFieldIrq(system);
        }
        break;
    case 4:
        FieldSystem_PreparePauseScene(system);
        system->task.status = 4;
        system->scene_transition.phase = 0;
        return 1;
    case 5:
        if (!FieldSystem_IsSaveFadeActive(system)) {
            system->scene_transition.phase = 6;
            FreezeFieldIrq(system);
        }
        break;
    case 6:
        FieldSystem_PrepareSaveScene(system);
        system->task.status = 5;
        system->scene_transition.phase = 0;
        return 1;
    case 7:
        if (!FieldSystem_IsShopFadeActive(system)) {
            system->scene_transition.phase = 8;
            FreezeFieldIrq(system);
        }
        break;
    case 8:
        FieldSystem_PrepareShopScene(system);
        system->task.status = 6;
        system->scene_transition.phase = 0;
        return 1;
    case 9:
        if (!FieldSystem_IsCreditsFadeActive(system))
            system->scene_transition.phase = 10;
        break;
    case 10:
        FieldSystem_ReleaseForCredits(system);
        system->task.status = 7;
        system->scene_transition.phase = 0;
        return 1;
    case 11:
        if (!FieldSystem_IsGameOverFadeActive(system))
            system->scene_transition.phase = 12;
        break;
    case 12:
        FieldSystem_ReleaseForGameOver(system);
        system->task.status = 8;
        system->scene_transition.phase = 0;
        return 1;
    }
    return 0;
}

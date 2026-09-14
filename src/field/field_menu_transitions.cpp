#include "field_scene_release_internal.h"

extern "C" {
void FieldSystem_RequestPause(FieldSystem *system, int option, int formation, int parameter,
                              u8 fade, u8 side) {
    func_0202cbd4(REQUEST, 0, sizeof(*REQUEST));
    SAVE->request.formation = formation;
    *(s16 *)SAVE->request.unknown_02 = parameter;
    SAVE->request.unknown_02[2] = 0;
    SAVE->bits.exit.raw = (SAVE->bits.exit.raw & ~0x8000) | ((side & 1) << 15);
    if (!option)
        SAVE->bits.exit.raw &= ~0x7fff;
    else
        SAVE->bits.exit.raw = (SAVE->bits.exit.raw & ~0x7fff) | 1;
    SuspendAreaTasks(system);
    if (fade) {
        FadeBoth(system);
        system->scene_transition.phase = 3;
    } else
        system->scene_transition.phase = 4;
}

int FieldSystem_IsPauseFadeActive(FieldSystem *system) { return IsFadeActive(system); }

void FieldSystem_PreparePauseScene(FieldSystem *system) { StorePersistentTransition(system); }

void FieldSystem_RequestSave(FieldSystem *system, int formation, int fade, int side) {
    func_0202cbd4(REQUEST, 0, sizeof(*REQUEST));
    SAVE->request.formation = formation;
    *(s16 *)SAVE->request.unknown_02 = 0;
    SAVE->request.unknown_02[2] = 0;
    SAVE->bits.exit.raw &= ~0x7fff;
    SAVE->bits.exit.raw = (SAVE->bits.exit.raw & ~0x8000) | ((side & 1) << 15);
    ((u8 *)&SAVE->request.flags)[0] = system->save_location;
    SuspendAreaTasks(system);
    if (fade) {
        FadeBoth(system);
        system->scene_transition.phase = 5;
    } else
        system->scene_transition.phase = 6;
}

int FieldSystem_IsSaveFadeActive(FieldSystem *system) { return IsFadeActive(system); }

void FieldSystem_PrepareSaveScene(FieldSystem *system) { StorePersistentTransition(system); }

void FieldSystem_RequestShop(FieldSystem *system, int formation, int fade, int side) {
    func_0202cbd4(REQUEST, 0, sizeof(*REQUEST));
    SAVE->request.formation = formation;
    SAVE->request.unknown_02[0] &= ~15;
    ((u8 *)&SAVE->request.flags)[1] = (((u8 *)&SAVE->request.flags)[1] & ~1) | (side & 1);
    SuspendAreaTasks(system);
    if (fade) {
        FadeBoth(system);
        system->scene_transition.phase = 7;
    } else
        system->scene_transition.phase = 8;
}

int FieldSystem_IsShopFadeActive(FieldSystem *system) { return IsFadeActive(system); }

void FieldSystem_PrepareShopScene(FieldSystem *system) {
    func_ov000_0206a130(system);
    data_0205a01c.first = 47;
    data_0205a01c.second = 70;
    data_0205a01c.side = SAVE->bits.flags_high.side;
}

void FieldSystem_RequestCredits(FieldSystem *system, int fade) {
    if (fade) {
        FadeBoth(system);
        system->scene_transition.phase = 9;
    } else
        system->scene_transition.phase = 10;
}

int FieldSystem_IsCreditsFadeActive(FieldSystem *system) { return IsFadeActive(system); }

void FieldSystem_ReleaseForCredits(FieldSystem *system) { ReleaseResources(system); }
}

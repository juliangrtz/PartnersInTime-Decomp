#include "field_scene_release_internal.h"

extern "C" {
void FieldSystem_RequestGameOver(FieldSystem *system, int fade) {
    func_0202cbd4(REQUEST, 0, sizeof(*REQUEST));
    if (fade) {
        FadeBoth(system);
        system->scene_transition.phase = 11;
    } else
        system->scene_transition.phase = 12;
}

int FieldSystem_IsGameOverFadeActive(FieldSystem *system) { return IsFadeActive(system); }

void FieldSystem_ReleaseForGameOver(FieldSystem *system) { ReleaseResources(system); }
}

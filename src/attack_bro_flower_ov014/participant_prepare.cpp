/* Select a projectile and start its one-time effects, overlay 14, 0x020C4F10-0x020C50A0. */
#include "flower_internal.h"
extern "C" {
extern s8 data_ov014_020c6204[];

void Overlay14Participant_PrepareProjectile(Overlay14Participant *participant, int mode)
{
    BattleSceneObject *actor = participant->actor->actor.scene_object;
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    if (!participant->projectile) {
        int slot = Overlay14Attack_FindFreeProjectile();
        if (slot == -1)
            return;
        participant->projectile = &work->projectiles[slot];
    }
    if (mode == -1)
        mode = work->active == participant ? 2 : 1;
    Overlay14Participant_SetProjectileMode(participant, mode);
    if (!participant->flags.effect_started) {
        const s8 *offset;
        int model_effect, sprite_effect;
        if (mode >= 2) {
            offset = data_ov014_020c6204 + 10 * participant->actor->formation_index + 7;
            model_effect = 846;
            sprite_effect = work->variant ? 541 : 539;
        } else {
            offset = data_ov014_020c6204 + 10 * participant->actor->formation_index;
            model_effect = 847;
            sprite_effect = work->variant ? 542 : 540;
        }
        BattleSpriteEffect_SpawnRelative(sprite_effect, actor, offset[0], -offset[1], 0, 256);
        BattleModelEffect_SpawnAttached(&participant->effect, model_effect,
                                       &participant->projectile->object, 0, 0, 0, 256);
        BattleSound_Play(0, 0, 0, 0);
        participant->flags.effect_started = 1;
    }
}
}

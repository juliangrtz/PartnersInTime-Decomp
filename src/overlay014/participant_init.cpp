#include <game/overlay014_projectile.h>
extern "C" {
#include <game/battle_effect.h>
extern u8 *data_ov002_020c0710;
extern void BattleParty_RestoreIdleAnimation(BattlePartyActor *, BattleSceneObject *);
void Overlay14Participant_Initialize(Overlay14Participant *participant, BattlePartyActor *actor, s16 resource,
                                     s8 side)
{
    participant->actor = actor;
    participant->projectile = 0;
    participant->timer = 0;
    participant->resource = resource;
    participant->phase = 0;
    participant->remaining = 0;
    participant->flags.side = (u8)side;
}

void Overlay14Participant_RestoreActor(Overlay14Participant *participant)
{
    if (participant->actor)
        BattleParty_RestoreIdleAnimation(participant->actor, participant->actor->actor.scene_object);
}
}

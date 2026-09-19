#include "flower_internal.h"

extern "C" {
void Overlay14Attack_UpdateEntry(Overlay10ActionActor *user)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    BattleSceneObject_GetById(40);
    func_ov014_020c53d4(&work->participants[0], &work->participants[1]);
    func_ov014_020c53d4(&work->participants[1], &work->participants[0]);
    Overlay14Attack_Update(&work->attack);
    for (int i = 0; i < 32; ++i)
        func_ov014_020c4464(&work->projectiles[i]);
    switch (work->common.phase) {
    case 0:
        Overlay14Attack_StartApproach(&work->attack);
        work->common.timer = 0;
        work->common.phase = 1;
        break;
    case 1:
        if (Overlay14Attack_IsIdle(&work->attack)) {
            Overlay14Attack_FinishWithEffects(&work->attack);
            work->common.timer = 0;
            work->common.phase = 2;
        }
        break;
    case 2:
        ++work->common.timer;
        if (work->common.timer >= 8) {
            BattleParty_RestoreIdleAnimation(user, user->party.actor.scene_object);
            work->common.phase = 3;
        }
        break;
    case 3:
        ++work->common.timer;
        if (work->common.timer >= 8) {
            for (int i = 0; i < 2; ++i)
                Overlay14Participant_BeginEntry(&work->participants[i]);
            work->common.phase = 4;
        }
        break;
    case 4:
        if (Overlay14Participant_IsIdle(&work->participants[0]) &&
            Overlay14Participant_IsIdle(&work->participants[1])) {
            work->active = &work->participants[0];
            work->next = 0;
            work->participants[0].remaining = func_ov014_020c2af8();
            func_ov014_020c4f10(&work->participants[0], -1);
            func_ov014_020c4f10(&work->participants[1], -1);
            work->participants[0].flags.flag1 = 1;
            work->participants[1].flags.flag1 = 1;
            Overlay14Attack_LaunchAtTarget(&work->attack, (Overlay14AttackTargetReference *)work->active);
            work->common.phase = 0;
            func_ov002_020722ac(user, Overlay14Attack_UpdateSequence);
        }
        break;
    }
}

void Overlay14Attack_UpdateSequence(Overlay10ActionActor *user)
{
    Overlay14Work *work = (Overlay14Work *)data_ov002_020c0710;
    if (work->common.phase <= 2 && !Overlay14Attack_PickTarget(0)) {
        ((Overlay14AttackFlags *)&work->common.flags)->stop = 1;
        work->common.phase = 2;
    }
    int first = 0, second = 1;
    if (work->active != &work->participants[0] && work->active == &work->participants[1]) {
        first = 1;
        second = 0;
    }
    Overlay14Participant *active, *other;
    other = &work->participants[second];
    active = &work->participants[first];
    func_ov014_020c53d4(active, other);
    func_ov014_020c53d4(other, active);
    for (int i = 0; i < 32; ++i)
        func_ov014_020c4464(&work->projectiles[i]);
    Overlay14Attack_Update(&work->attack);
    switch (work->common.phase) {
    case 0:
    case 1:
    case 2:
        if (!work->active && --work->delay <= 0) {
            work->active = work->next;
            work->next = 0;
            Overlay14Participant *participant = work->active;
            if (participant->projectile && participant->projectile->flags.level)
                Overlay14Participant_SetProjectileMode(participant, 2);
            participant->remaining = func_ov014_020c2af8();
        }
        if (Overlay14Participant_IsIdle(&work->participants[0]) &&
            Overlay14Participant_IsIdle(&work->participants[1]) &&
            Overlay14Projectiles_AreIdle(work->projectiles, 32)) {
            Overlay14Attack_BeginHide(&work->attack);
            Overlay14Participant_EndFade(&work->participants[0]);
            Overlay14Participant_EndFade(&work->participants[1]);
            work->common.timer = 0;
            work->common.phase = 3;
        }
        break;
    case 3:
        if (work->common.timer < 0) {
            ++work->common.timer;
            return;
        }
        {
            for (int i = 0; i < 6; ++i) {
                BattleActor_GetById((u16)(i + 60));
                BattleSceneObject_GetById((u16)(i + 60));
                if (Overlay10Enemy_IsSelectable((u16)(i + 60)))
                    func_ov014_020c2d24((u16)(i + 60), 1);
            }
            BattleSound_Play(data_ov014_020c6158[work->variant][work->rating], 0, 0, 0);
            switch (work->feedback) {
            case 1:
                BattleSound_Play(49, 0, 0, 0);
                break;
            case 2:
                BattleSound_Play(49, 0, 0, 0);
                break;
            }
            BattleScreenEffect_StartPrimary(data_ov014_020c61b0[work->rating].value,
                                            data_ov014_020c61b2[work->rating].value,
                                            data_ov014_020c61b4[work->rating].value);
            work->common.timer = 0;
            work->common.phase = 4;
        }
        break;
    case 4:
        if (work->common.timer < 56)
            ++work->common.timer;
        else {
            work->common.timer = 0;
            work->common.phase = 5;
        }
        break;
    case 5:
        for (int i = 0; i < 2; ++i)
            Overlay14Participant_RestoreActor(&work->participants[i]);
        for (int i = 0; i < 32; ++i)
            Overlay14Projectile_ReleaseModel(&work->projectiles[i].object);
        Overlay14Attack_Stop(&work->attack);
        Overlay14Attack_ReleaseEnemyEffects();
        func_ov002_020722ac(user, 0);
        data_ov002_020c0710 = 0;
        return;
    }
    ++work->timer;
    if (work->common.phase == 0 && work->timer >= data_ov014_020c6124) {
        Overlay14Participant_StartFade(&work->participants[0]);
        Overlay14Participant_StartFade(&work->participants[1]);
        work->common.phase = 1;
    }
    if (work->common.phase == 1 && work->timer >= data_ov014_020c6124 + 180) {
        for (int i = 0; i < 2; ++i)
            Overlay14Participant_BeginRecovery(&work->participants[i]);
        Overlay14Attack_BeginHide(&work->attack);
        work->common.phase = 2;
    }
}
}

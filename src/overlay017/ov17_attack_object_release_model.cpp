#include "participant_internal.h"

extern "C" {
void Overlay17Participant_AppendToList(Overlay17Participant *participant)
{
    Overlay17Participant *tail = data_ov002_020c0710->head;
    if (!tail) {
        data_ov002_020c0710->head = participant;
        return;
    }
    for (Overlay17Participant *next = tail->state.next; next; next = next->state.next)
        tail = next;
    tail->state.next = participant;
    participant->state.next = 0;
}

void Overlay17Participant_RemoveIdleFromList(void)
{
    Overlay17BattleStateView *work = data_ov002_020c0710;
    Overlay17Participant *participant = work->head;
    if (!participant)
        return;
    Overlay17Participant *previous = 0;
    for (; participant; participant = participant->state.next) {
        if (!participant->state.bits.phase) {
            Overlay17Participant *next = participant->state.next;
            participant->state.next = 0;
            if (previous)
                previous->state.next = next;
            if (participant == work->head)
                work->head = next;
        }
        previous = participant;
    }
}

void Overlay17AttackObject_Initialize(Overlay17AttackObject *attack_object, int group, int phase, int animation_component,
                                      int enabled)
{
    if (enabled) {
        attack_object->animation_component = 48 * animation_component + 2444;
        attack_object->flag_bits.enabled = 1;
    }
    attack_object->scale_x = 256;
    attack_object->scale_y = 256;
    attack_object->flag_bits.resource_variant = group;
    attack_object->flag_bits.animation_phase = phase;
    attack_object->object.render_state = 32767;
    attack_object->object.effect_anchor_z = -8;
}

void Overlay17AttackObject_InitializeGroup(Overlay17AttackObject *attack_object, int group)
{
    int i = 0;
    int animation_component = 2 * group;
    do {
        Overlay17AttackObject_Initialize(attack_object, group, i & 1, animation_component, i < 2);
        ++i;
        ++animation_component;
        ++attack_object;
    } while (i < 10);
}

void Overlay17AttackObject_ReleaseModel(Overlay17AttackObject *attack_object)
{
    attack_object->flag_bits.state = 0;
    if (attack_object->object.primary_model) {
        func_ov002_0206f1b8(attack_object->object.primary_model);
        if (attack_object->object.primary_model)
            attack_object->object.primary_model->unknown_14();
        attack_object->object.primary_model = 0;
    }
}
}

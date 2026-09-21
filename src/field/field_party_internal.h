#ifndef PIT_FIELD_PARTY_INTERNAL_H
#define PIT_FIELD_PARTY_INTERNAL_H

/*
 * Declarations shared by the field party units.
 */
#include <game/field_resources.h>
#include <game/field_party.h>
#include <game/field_room_interaction.h>
#include <game/field_linear.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
extern "C" {
#include <game/audio.h>

int VM_ReadVariable(int, int, int);
void func_020093b4(FieldRenderObject *, int);
int func_ov000_020ae5fc(FieldRuntimeEntity *, FieldRuntimeEntity *);
void FieldEntity3D_StartBounceShake(FieldRuntimeEntity *);

extern const FieldPartyTrailDirection data_ov000_020c068c[];
void func_ov000_0208dbd4(FieldPartyController *, FieldRuntimeEntity **, int);

extern const u8 data_ov000_020c0684[];
extern const FieldPartyTrailDirection data_ov000_020c06ac[];
extern const u8 data_ov000_020c06a4[];
void func_ov000_02092e24(FieldPartyController *, FieldPartyEntity *);
}
#endif

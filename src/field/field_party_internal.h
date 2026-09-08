#ifndef PIT_FIELD_PARTY_INTERNAL_H
#define PIT_FIELD_PARTY_INTERNAL_H
#include <game/field_party.h>
#include <game/field_linear.h>
#include <game/field_entity_lifecycle.h>
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
extern "C" {
#include <game/audio.h>
void func_ov000_02093108(FieldPartyController *, FieldPartyEntity *, fx32, fx32 *, fx32 *);

int VM_ReadVariable(int, int, int);
void func_ov000_02092f30(FieldPartyController *, FieldPartyEntity *, int, int, u8);
void func_ov000_0209ce18(FieldPartyController *);
void func_ov000_0209ce44(FieldPartyController *, int, int);
void func_020093b4(FieldRenderObject *, int);
void func_ov000_020922a4(FieldPartyController *, FieldPartyEntity *, int, int, int, int, int);
int func_ov000_020ae5fc(FieldRuntimeEntity *, FieldRuntimeEntity *);
void func_ov000_020b46d4(FieldRuntimeEntity *);

void func_ov000_02091d34(FieldPartyController *, FieldPartyEntity *);
void *func_ov000_0207f080(FieldResourceContext *, FieldPartyEntity *);
void func_ov000_0207ef18(FieldResourceContext *, void *);
void func_ov000_02091e58(FieldPartyController *, FieldPartyEntity *, fx32, fx32, fx32, int, u8, s16, int, int,
                         int, const s16 *);
extern const s16 data_ov000_020c068c[];
void func_ov000_0208dbd4(FieldPartyController *, FieldRuntimeEntity **, int);

extern const u8 data_ov000_020c0684[];
extern const s16 data_ov000_020c06ac[];
}
#endif

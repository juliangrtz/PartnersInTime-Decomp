extern "C" {
#include <game/heap.h>
}
#include <game/field_party.h>
#include <game/field_auxiliary.h>
extern "C" {
extern FieldEntityVTable data_ov000_020c1244;
void func_0202cbd4(void *, int, u32);
FieldPartyEntity *FieldPartyEntity_InitEmpty(FieldPartyEntity *party) {
    FieldEntity3D_Init(&party->entity);
    *(FieldEntityVTable **)party = &data_ov000_020c1244;
    func_0202cbd4((u8 *)party + 4, 0, sizeof(*party) - 4);
    return party;
}
FieldPartyEntity *FieldPartyEntity_Destroy(FieldPartyEntity *party) {
    *(FieldEntityVTable **)party = &data_ov000_020c1244;
    for (int i = 0; i < 6; ++i) {
        FieldAuxiliaryEntity *aux = party->auxiliaries[i];
        if (aux)
            aux->entity.base.unknown_04();
    }
    FieldEntity3D_DestroyBase(&party->entity);
    return party;
}
FieldPartyEntity *FieldPartyEntity_Delete(FieldPartyEntity *party) {
    *(FieldEntityVTable **)party = &data_ov000_020c1244;
    for (int i = 0; i < 6; ++i) {
        FieldAuxiliaryEntity *aux = party->auxiliaries[i];
        if (aux)
            aux->entity.base.unknown_04();
    }
    FieldEntity3D_DestroyBase(&party->entity);
    GameHeap_Delete(party);
    return party;
}
}

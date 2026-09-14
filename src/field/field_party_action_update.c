#include <game/field_party_manager.h>
extern void func_ov000_0209c930(FieldPartyController *);
extern void func_ov000_020a03a0(FieldPartyManager *);
void FieldPartyManager_UpdateActions(FieldPartyManager *manager) {
    func_ov000_0209c930(&manager->parties[manager->bits.active_party]);
    func_ov000_0209c930(&manager->parties[manager->bits.active_party ^ 1]);
    func_ov000_020a03a0(manager);
    FieldPartyManager_UpdateRegion71ActionModels(manager);
}

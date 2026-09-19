/*
 * Party action update (overlay 0, 0x020A04E4-0x020A0544).
 *
 * Steps the party's active actions once per frame.
 */

#include <game/field_party_manager.h>
extern void func_ov000_0209c930(FieldPartyController *);
void FieldPartyManager_UpdateActions(FieldPartyManager *manager) {
    func_ov000_0209c930(&manager->parties[manager->bits.active_party]);
    func_ov000_0209c930(&manager->parties[manager->bits.active_party ^ 1]);
    FieldPartyManager_UpdateContactHint(manager);
    FieldPartyManager_UpdateRegion71ActionModels(manager);
}

#include <game/field_party_manager.h>
typedef struct PartyOwnerModeView {
    u8 prefix[600];
    u16 mode : 4, reserved : 12;
} PartyOwnerModeView;
typedef char PartyOwnerModeView_SizeCheck[sizeof(PartyOwnerModeView) == 602 ? 1 : -1];

extern "C" void FieldPartyManager_UpdateRegion71ActionModels(FieldPartyManager *manager) {
    if (!manager->owner || !((PartyOwnerModeView *)manager->owner)->mode) {
        FieldParty_UpdateRegion71ActionModel(&manager->parties[manager->bits.active_party], 0);
        FieldParty_UpdateRegion71ActionModel(&manager->parties[manager->bits.active_party], 1);
        FieldParty_UpdateRegion71ActionModel(&manager->parties[manager->bits.active_party ^ 1], 0);
        FieldParty_UpdateRegion71ActionModel(&manager->parties[manager->bits.active_party ^ 1], 1);
    }
}

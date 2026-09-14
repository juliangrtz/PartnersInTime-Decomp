#include <game/field_party_manager.h>
#include <game/script_vm.h>
typedef union PartyPresence {
    u32 raw;
    struct {
        u32 leader : 1, follower : 1, unknown : 30;
    } bits;
} PartyPresence;
#define PRESENCE(m, i) ((PartyPresence *)&(m)->parties[i].flags)
void FieldPartyManager_ApplyStoryPresence(FieldPartyManager *manager, int select_active) {
    if (VM_ReadVariable(8197, 0, 0) || VM_ReadVariable(8198, 0, 0)) {
        if (select_active)
            manager->bits.active_party = 0;
        PRESENCE(manager, 0)->bits.leader = 1;
        PRESENCE(manager, 0)->bits.follower = VM_ReadVariable(8197, 0, 0) == 0;
        PRESENCE(manager, 1)->bits.leader = 0;
        PRESENCE(manager, 1)->bits.follower = 0;
        if (VM_ReadVariable(8197, 0, 0))
            manager->parties[0].flags.movement_active = 0;
        manager->parties[1].flags.movement_state = 13;
    } else if (VM_ReadVariable(8199, 0, 0)) {
        if (select_active)
            manager->bits.active_party = 1;
        PRESENCE(manager, 0)->bits.leader = 0;
        PRESENCE(manager, 0)->bits.follower = 0;
        PRESENCE(manager, 1)->bits.leader = 1;
        PRESENCE(manager, 1)->raw |= 2;
        manager->parties[0].flags.movement_state = 13;
    } else {
        if (select_active)
            manager->bits.active_party = 0;
        PRESENCE(manager, 0)->bits.leader = 1;
        PRESENCE(manager, 0)->raw |= 2;
        PRESENCE(manager, 1)->bits.leader = 1;
        PRESENCE(manager, 1)->raw |= 2;
    }
    manager->parties[manager->bits.active_party].flags.unknown_03 = 1;
    manager->parties[manager->bits.active_party ^ 1].flags.unknown_03 = 0;
}

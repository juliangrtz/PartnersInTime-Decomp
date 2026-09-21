/* Restore the saved contact byte, then suppress its low four contact flags
 * while this member or its linked member is following. */
#include <game/field_party.h>
void FieldPartyEntity_RestoreContactFlags(FieldPartyEntity *member)
{
    FieldPartyFollowerFlags *flags = 0;
    member->entity.collision_flag_bytes.current = member->entity.collision_flag_bytes.saved;
    if (!member->follower_flags) {
        if (member->linked_member)
            flags = member->linked_member->follower_flags;
    } else {
        flags = member->follower_flags;
    }
    if (flags && flags->enabled) {
        member->entity.collision_state_flag_bits.unknown_00 = 0;
        member->entity.collision_state_flag_bits.unknown_01 = 0;
        member->entity.collision_state_flag_bits.unknown_02 = 0;
        member->entity.collision_state_flag_bits.unknown_03 = 0;
    }
}

/* Update non-null follower bindings and disable following until initialized.
 * A state must already be bound when the state argument is null.
 */
#include <game/field_party.h>
extern "C" void FieldParty_BindFollowerState(FieldPartyEntity *member, void *state,
                                             FieldPartyEntity *target)
{
    if (state)
        member->follower_flags = (FieldPartyFollowerFlags *)state;
    if (target)
        member->following_target = target;
    member->follower_flags->enabled = 0;
}

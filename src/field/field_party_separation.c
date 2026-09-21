/* Retain the full fixed-point separation for formation transitions. Unsigned
 * subtraction preserves the native 32-bit wrap at the coordinate limits. */
#include <game/field_party.h>
void FieldParty_UpdateSeparation(FieldPartyController *party)
{
    party->separation_x = (u32)party->leader->entity.position_x - (u32)party->follower->entity.position_x;
    party->separation_y = (u32)party->leader->entity.position_y - (u32)party->follower->entity.position_y;
    party->separation_z = (u32)party->leader->entity.position_z - (u32)party->follower->entity.position_z;
}

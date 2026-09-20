/* Palette animation slot 0 on the party leader. The active bit suppresses
 * redundant rebinding; force bypasses that guard. Special contact mode selects
 * animation 3 instead of 2. Clearing requests a reload of the base palette.
 */
#include <game/field_party.h>
extern "C" {
void FieldParty_StartLeaderPaletteAnimation(FieldPartyController *party, int force)
{
    if (force || !party->flags.palette_animation_active) {
        party->leader->entity.render_object->bind_palette_animation(
            0, party->flags.special_contact_mode + 2, 1);
        party->leader->entity.render_object->set_palette_animation_paused(0, 0);
        party->leader->entity.render_object->set_palette_animation_mode(0, 1);
        party->flags.palette_animation_active = 1;
    }
}
void FieldParty_ClearLeaderPaletteAnimation(FieldPartyController *party, int force)
{
    if (force || party->flags.palette_animation_active) {
        party->leader->entity.render_object->bind_palette_animation(0, -1, 1);
        party->leader->entity.render_object->request_base_palette_reload();
        party->flags.palette_animation_active = 0;
    }
}
}

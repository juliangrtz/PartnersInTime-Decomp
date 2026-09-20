/* Rebind a field sprite to a room palette and restart its palette controller.
 * The selector must resolve to a palette in one of the two resource sets.
 * Standard palettes use an inclusive first..first+count interval; alternate
 * palettes compare the selector's low nibble with their explicit bank.
 */
#include <game/field_resources.h>
extern "C" void FieldEntity_SetPaletteProfile(FieldResourceContext *field,
                                              FieldRuntimeEntity *entity, int selector)
{
    FieldPaletteResource *palette;
    FieldSecondaryResource *secondary = 0;
    if (selector >= 16) {
        entity->presentation_state = (selector & 15) | 0x80;
        for (int set = 0; set < 2; ++set) {
            palette = field->alternate_palettes[set];
            for (int index = 0; index < field->alternate_counts[set]; ++index, ++palette) {
                if (palette->palette.bank == (selector & 15)) {
                    secondary = &field->alternate[set][index];
                    goto found;
                }
            }
        }
    } else {
        entity->presentation_state = selector;
        for (int set = 0; set < 2; ++set) {
            palette = field->palettes[set];
            for (int index = 0; index < field->secondary_counts[set]; ++index, ++palette) {
                int first = palette->palette.first;
                if (first <= selector && first + palette->palette.count >= selector) {
                    secondary = &field->secondary[set][index];
                    goto found;
                }
            }
        }
    }
found:
    entity->render_object->unknown_060 = (u8)(selector & 15);
    entity->render_object->palette = &palette->palette;
    entity->render_object->animation_controller = secondary->auxiliary;
    entity->render_object->reset_controller_work();
    entity->render_object->set_controller_animation(entity->render_object->resource_animation, 0);
    entity->render_object->bind_palette_animation(-1, -1, 1);
}

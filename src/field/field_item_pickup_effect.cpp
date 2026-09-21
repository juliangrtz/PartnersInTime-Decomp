/*
 * Entity item pickup presentation (overlay 0, 0x0206F0B4-0x0206F110).
 * Combine projected coordinates with entity offsets before narrowing to the
 * signed-halfword position consumed by the shared pickup effect.
 */
#include <game/field_area.h>
#include <game/field_variable_entity.h>
#include <game/field_room_interaction.h>

extern "C" {
void func_ov000_0206ec68(FieldAreaContext *, int, int, s16, s16, int);

void FieldArea_ShowEntityItemPickup(FieldAreaContext *area, FieldVariableEntity *object, int added)
{
    func_ov000_0206ec68(area, object->state.item, object->state.amount_index,
                       object->entity.screen_x + object->entity.screen_offset_x,
                       object->entity.screen_y + object->entity.screen_offset_y, added);
}
}

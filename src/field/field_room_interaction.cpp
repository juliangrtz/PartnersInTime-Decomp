#include <game/field_area.h>
#include <game/field_room_interaction.h>
#include <game/field_party_manager.h>
#include <game/field_entity_motion.h>
#include <game/field_presentation.h>
#include <game/field_variable_entity.h>

extern "C" {
#include <game/inventory.h>

enum {
    FIELD_PLACEMENT_BEAN = 0x200d,
    FIELD_VARIABLE_COLLECTED_BEAN = 0x201f,
    FIELD_VARIABLE_BANK = 0x2000,
    FIELD_INVENTORY_BEANS = 0xfffe,
    FIELD_INVENTORY_COINS = 0xffff,
    FIELD_PLACEMENT_HALF_EXTENT = 8
};

extern const s8 data_ov000_020bfbfc[], data_ov000_020bfc1c[];
void func_ov000_0207edc0(FieldAreaContext *, int, int);
void func_ov000_0206f0b4(FieldAreaContext *, FieldVariableEntity *, int);
void func_ov000_0206ec68(FieldAreaContext *, int, int, s16, s16, int);
void func_ov000_0207bed8(FieldAreaContext *, int, int);
void func_ov000_02079d74(FieldAreaContext *, int);
void func_ov000_020b7b68(FieldPartyEntity *, const FieldQuadRegion *);

void FieldArea_EnsurePartyPresent(FieldAreaContext *field, int side, int x, int y, s16 z, int direction)
{
    if (field->party->parties[side].backup.active ||
        field->room_id != field->party->parties[side].flags.movement_state) {
        field->flags.mode |= 1 << side;
        func_ov000_0207bed8(field, side, 0);
        field->party->parties[side].flags.unknown_02 = 1;
        FieldPartyManager_SetFieldMode(field->party, side, 0, 1, 0);
        field->party->parties[side].leader->entity.base.property_00a_bits.property_00a_flag_00 = 1;
        field->party->parties[side].leader->entity.base.set_visible(1);
        field->party->parties[side].follower->entity.base.property_00a_bits.property_00a_flag_00 = 1;
        field->party->parties[side].follower->entity.base.set_visible(1);
        FieldEntity3D_SetPosition(&field->party->parties[side].leader->entity, x << 12, y << 12, z << 12);
        FieldEntity_SetFacingDirection(&field->party->parties[side].leader->entity, 0, direction, 1);
        FieldParty_RejoinFollower(&field->party->parties[side], 1);
        func_ov000_02079d74(field, 1 << side);
        field->party->parties[side].flags.movement_state = field->room_id;
        func_ov000_020b7b68(field->party->parties[side].leader, field->quad_regions);
        func_ov000_020b7b68(field->party->parties[side].follower, field->quad_regions);
        field->party->parties[side].backup.active = 0;
    }
}

int FieldArea_CollectEntityItem(FieldAreaContext *field, FieldVariableEntity *entity)
{
    u16 item = entity->state.item;
    int amount;
    if (item == FIELD_INVENTORY_COINS)
        amount = data_ov000_020bfbfc[entity->state.amount_index];
    else
        amount = data_ov000_020bfc1c[entity->state.amount_index];
    int added = GameInventory_Add(item, amount) > 0;
    func_ov000_0207edc0(field, item, 0);
    func_ov000_0206f0b4(field, entity, added);
    if (added) {
        if (!--entity->state.remaining_uses)
            entity->state.motion.unknown_01_15 = 0;
        VM_WriteVariable(entity->entity.unknown_3d4, 1, 0, 0);
    }
    return added;
}

const FieldVariablePlacement *FieldArea_FindUncollectedPlacement(FieldAreaContext *field,
                                                                 FieldRuntimeEntity *entity)
{
    const FieldVariablePlacement *record = field->variable_records;
    /* The null-list and end-marker cases share the native final return. */
    if (record) {
        int x = entity->position_x, y = entity->position_y;
        int z = (s16)(entity->position_z / 4096);
        int max_x = (s16)((x + entity->navigation_max_x) / 4096);
        int min_x = (s16)((x + entity->navigation_min_x) / 4096);
        int max_y = (s16)((y + entity->navigation_max_y) / 4096);
        int min_y = (s16)((y + entity->navigation_min_y) / 4096);
        for (;;) {
            if (!record->flags.mode &&
                !VM_ReadVariable((u16)(record->variable + (u16)-FIELD_VARIABLE_BANK), 0, 0) &&
                record->z == z && record->x - FIELD_PLACEMENT_HALF_EXTENT <= max_x &&
                record->x + FIELD_PLACEMENT_HALF_EXTENT >= min_x &&
                record->y - FIELD_PLACEMENT_HALF_EXTENT <= max_y &&
                record->y + FIELD_PLACEMENT_HALF_EXTENT >= min_y)
                return record;
            if (record->flags.last)
                break;
            ++record;
        }
    }
    return 0;
}

int FieldArea_CollectPlacementItem(FieldAreaContext *field, const FieldVariablePlacement *record)
{
    u16 item = record->item;
    int amount;
    if (item == FIELD_INVENTORY_COINS)
        amount = data_ov000_020bfbfc[record->motion.amount_index];
    else
        amount = data_ov000_020bfc1c[record->motion.amount_index];
    /* Bean placements increment the separate inventory counter. */
    int added = GameInventory_Add(item == FIELD_PLACEMENT_BEAN ? FIELD_INVENTORY_BEANS : item, amount) > 0;
    func_ov000_0207edc0(field, item, 1);
    func_ov000_0206ec68(field, item, record->motion.amount_index,
                        record->x - (field->unknown_244c[0] - field->unknown_2464[0]),
                        record->y - record->z - (field->unknown_244c[1] - field->unknown_2464[1]), added);
    if (added) {
        VM_WriteVariable((u16)(record->variable + (u16)-FIELD_VARIABLE_BANK), 1, 0, 0);
        /* Mode-zero placements use the shared parameter as a navigation change index. */
        FieldArea_ApplyNavigationChange(field, record->flags.parameter);
        if (item == FIELD_PLACEMENT_BEAN)
            VM_WriteVariable(FIELD_VARIABLE_COLLECTED_BEAN, 1, 0, 0);
    }
    return added;
}
}

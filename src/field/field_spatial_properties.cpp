/* Spatial properties add vertical motion, contact and support information,
 * delegating the common properties to the 2D implementation. */
#include <game/field_entity_properties.h>
/* Entity types 0/1 are party members; type 10 is a party auxiliary. */
static inline int IsPartyKind(FieldRuntimeEntity *entity)
{
    int result = 0;
    unsigned kind = entity->base.property_00a_bits.subtype;
    if (kind <= 10 && ((1 << kind) & 0x403))
        result = 1;
    return result;
}
static inline int IsOtherKind(FieldRuntimeEntity *entity)
{
    int result = 1;
    unsigned kind = entity->base.property_00a_bits.subtype;
    if (kind <= 10 && ((1 << kind) & 0x403))
        result = 0;
    return result;
}
extern "C" int FieldEntity3D_GetProperty(FieldRuntimeEntity *entity, int property)
{
    switch (property) {
    case FIELD_PROPERTY_ENABLED:
    case FIELD_PROPERTY_INTERACTION_STATE:
        return FieldEntity_GetInteractionState(&entity->base, property);
    case FIELD_PROPERTY_PLANAR_MOTION:
    case FIELD_PROPERTY_ANIMATION_ENABLED:
    case FIELD_PROPERTY_OUTSIDE_SCREEN:
    case FIELD_PROPERTY_RENDER_LAYER:
    case FIELD_PROPERTY_POSITION_X:
    case FIELD_PROPERTY_POSITION_Y:
    case FIELD_PROPERTY_FACING:
    case FIELD_PROPERTY_SCREEN_X:
    case FIELD_PROPERTY_SCREEN_Y:
    case FIELD_PROPERTY_MOVEMENT_SPEED:
    case FIELD_PROPERTY_RESOURCE_INDEX:
    case FIELD_PROPERTY_RESOURCE_ANIMATION:
    case FIELD_PROPERTY_ANIMATION_ID:
    case FIELD_PROPERTY_ANIMATION_SPEED:
        return FieldEntity2D_GetProperty(entity, property);
    case FIELD_PROPERTY_VERTICAL_MOTION:
        return !entity->field_state_flag_bits.unknown_07 &&
               entity->field_state_flag_bits.vertical_motion_active;
    case FIELD_PROPERTY_FIELD_FLAG_07:
        return entity->field_state_flag_bits.unknown_07;
    case FIELD_PROPERTY_HAS_CONTACT_MASK_B:
        return entity->runtime_flag_bits.contact_mask_b != 0;
    case FIELD_PROPERTY_RELATED_PARTY: {
        int result = 0;
        if (entity->unknown_500) {
            if (IsPartyKind((FieldRuntimeEntity *)entity->unknown_500))
                result = 1;
        }
        return result;
    }
    case FIELD_PROPERTY_RELATED_OTHER: {
        int result = 0;
        if (entity->unknown_500) {
            if (IsOtherKind((FieldRuntimeEntity *)entity->unknown_500))
                result = 1;
        }
        return result;
    }
    case FIELD_PROPERTY_POSITION_Z:
        return entity->position_z / 4096;
    case FIELD_PROPERTY_FIRST_MASK_BIT: {
        int mask = entity->unknown_3a8;
        for (int i = 0; i < 32; ++i, mask >>= 1)
            if (mask & 1)
                return i;
        return -1;
    }
    case FIELD_PROPERTY_SUPPORT_INDEX:
        return entity->support_entity_index;
    default:
        return 0;
    }
}

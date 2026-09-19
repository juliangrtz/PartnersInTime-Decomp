#ifndef PIT_GAME_FIELD_ENTITY_PROPERTIES_H
#define PIT_GAME_FIELD_ENTITY_PROPERTIES_H
#include <game/field_entity.h>

/* Script-visible property IDs. Positions are integer pixels; movement speed
 * retains its controller units. Renderer properties require a live renderer.
 * Properties 7/8 classify the linked entity at +0x500; 15 scans the +0x3A8 mask. */
enum FieldEntityProperty {
    FIELD_PROPERTY_PLANAR_MOTION = 0,
    FIELD_PROPERTY_VERTICAL_MOTION = 1,
    FIELD_PROPERTY_FIELD_FLAG_07 = 2,
    FIELD_PROPERTY_ANIMATION_ENABLED = 3,
    FIELD_PROPERTY_ENABLED = 4,
    FIELD_PROPERTY_INTERACTION_STATE = 5,
    FIELD_PROPERTY_HAS_CONTACT_MASK_B = 6,
    FIELD_PROPERTY_RELATED_PARTY = 7,
    FIELD_PROPERTY_RELATED_OTHER = 8,
    FIELD_PROPERTY_OUTSIDE_SCREEN = 9,
    FIELD_PROPERTY_RENDER_LAYER = 10,
    FIELD_PROPERTY_POSITION_X = 11,
    FIELD_PROPERTY_POSITION_Y = 12,
    FIELD_PROPERTY_POSITION_Z = 13,
    FIELD_PROPERTY_FACING = 14,
    FIELD_PROPERTY_FIRST_MASK_BIT = 15,
    FIELD_PROPERTY_SUPPORT_INDEX = 16,
    FIELD_PROPERTY_SCREEN_X = 17,
    FIELD_PROPERTY_SCREEN_Y = 18,
    FIELD_PROPERTY_MOVEMENT_SPEED = 19,
    FIELD_PROPERTY_RESOURCE_INDEX = 20,
    FIELD_PROPERTY_RESOURCE_ANIMATION = 21,
    FIELD_PROPERTY_ANIMATION_ID = 22,
    FIELD_PROPERTY_ANIMATION_SPEED = 23,
};

#ifdef __cplusplus
extern "C" {
#endif
int FieldEntity_GetInteractionState(FieldEntity *entity, int property);
int FieldEntity2D_GetProperty(FieldRuntimeEntity *entity, int property);
int FieldEntity3D_GetProperty(FieldRuntimeEntity *entity, int property);
#ifdef __cplusplus
}
#endif
#endif

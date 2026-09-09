#ifndef PIT_GAME_FIELD_VARIABLE_ENTITY_H
#define PIT_GAME_FIELD_VARIABLE_ENTITY_H

#include <game/field_entity_lifecycle.h>
#include <game/script_vm.h>
/* State selected by a room placement and persistent script variable.
 * The specific gameplay object class is not yet confirmed. */
typedef struct FieldVariableEntityState {
    struct {
        u32 mode : 2, enabled : 1, unknown_03 : 1, unknown_04 : 1, unknown_05 : 1;
        u32 unknown_06_13 : 8;
        s32 unknown_14_21 : 8;
        u32 unknown_22_31 : 10;
    } flags;
    struct {
        u16 unknown_00 : 1, unknown_01_15 : 15;
    } motion;
    union { u8 unknown_06, remaining_uses; };
    union { u8 unknown_07, amount_index; };
    union { u16 unknown_08, item; };
    u16 variable;
    u16 saved_animation, unknown_0e;
} FieldVariableEntityState;
typedef struct FieldVariableEntity {
    FieldRuntimeEntity entity;
    FieldVariableEntityState state;
} FieldVariableEntity;
typedef struct FieldVariablePlacement {
    struct {
        u8 last : 1, mode : 2, parameter : 5;
    } flags;
    struct {
        u8 amount_index : 5, direction : 1, unknown_06_07 : 2;
    } motion;
    union { u16 unknown_02, item; };
    u16 variable;
    s16 x, y, z;
} FieldVariablePlacement;
typedef char FieldVariableEntityState_SizeCheck[sizeof(FieldVariableEntityState) == 16 ? 1 : -1];
typedef char FieldVariableEntity_SizeCheck[sizeof(FieldVariableEntity) == 1328 ? 1 : -1];
typedef char FieldVariablePlacement_SizeCheck[sizeof(FieldVariablePlacement) == 12 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
FieldVariableEntity *FieldVariableEntity_CopyState(FieldVariableEntity *, const FieldVariableEntity *);
FieldVariableEntity *FieldVariableEntity_InitVariablePlacement(FieldVariableEntity *, int, int,
                                                               const FieldVariablePlacement *);
FieldVariableEntity *FieldVariableEntity_InitPlacement(FieldVariableEntity *, int, const FieldSpawnRecord *,
                                                       int, int, u8);
#ifdef __cplusplus
}
#endif
#endif

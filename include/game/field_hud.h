#ifndef PIT_GAME_FIELD_HUD_H
#define PIT_GAME_FIELD_HUD_H

#include <game/field_timed_renderer.h>

typedef struct FieldHudPartyGroup {
    u8 unknown_00[80];
    u32 active_members : 2, reserved : 30;
    u8 unknown_54[8272];
} FieldHudPartyGroup;

typedef struct FieldHudParty {
    u8 unknown_00[4];
    u32 active_group : 1, reserved : 31;
    u8 unknown_08[24];
    FieldHudPartyGroup groups[2];
} FieldHudParty;

/* Partial view of the field context used by the HUD, temporary effects and camera. */
typedef struct FieldHudContext {
    u8 unknown_0000[8];
    u32 digit_graphics;
    u8 unknown_000c[9288];
    fx32 camera_x, camera_y;
    u8 unknown_245c[44];
    struct {
        u16 active : 1, vertical : 1;
        s16 rumble : 8;
        u16 reserved : 6;
    } shake;
    u16 shake_repetitions;
    fx32 shake_step, shake_amplitude, shake_position, shake_origin;
    u8 unknown_249c[60];
    union {
        u16 raw;
        struct {
            u16 reserved : 14, visible : 1, moving : 1;
        };
    } hud;
    s16 hud_y, hud_step;
    u8 unknown_24de[142];
    FieldHudParty *party;
    u8 unknown_2570[1356];
    FieldTimedRenderer *renderers[27];
    u8 effect_owners[8];
} FieldHudContext;

typedef char FieldHudPartyGroup_SizeCheck[sizeof(FieldHudPartyGroup) == 8356 ? 1 : -1];
typedef char FieldHudContext_SizeCheck[sizeof(FieldHudContext) == 11056 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
int FieldEffect_FindSlot(void *field_context, FieldRuntimeEntity *entity);
void FieldEffect_StopForEntity(void *field_context, FieldRuntimeEntity *entity);
int FieldEffect_StartForEntity(void *field_context, FieldRuntimeEntity *entity, int slot, int animation,
                               int relative, s16 x, s16 y, int duration, u8 follow);
int FieldEffect_AnyActive(void *field_context);
void FieldEffect_StopAll(void *field_context);
void FieldEffect_Stop(void *field_context, int slot);
int FieldEffect_Start(void *field_context, int slot, int animation, s16 x, s16 y, int duration);
void FieldHud_UpdatePositions(void *field_context, int single);
GameOamEntry *FieldHud_Draw(void *field_context, GameOamEntry *output);
void FieldHud_SelectPortraits(void *field_context, int single);
void FieldHud_SetRenderersVisible(void *field_context, int visible, int single);
void FieldHud_Init(void *field_context, int visible);
void FieldCamera_StopShake(void *field_context);
#ifdef __cplusplus
}
#endif

#endif

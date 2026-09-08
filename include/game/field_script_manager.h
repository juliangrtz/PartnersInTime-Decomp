#ifndef PIT_GAME_FIELD_SCRIPT_MANAGER_H
#define PIT_GAME_FIELD_SCRIPT_MANAGER_H

#include <game/field_entity.h>
#ifdef __cplusplus
extern "C" {
#endif
#include <game/field_script.h>
#ifdef __cplusplus
}
#endif

typedef struct FieldScriptManager {
    s8 current_actor;
    u8 reserved[3];
    FieldVmRuntime runtimes[2];
    FieldScriptState states[4];
} FieldScriptManager;

typedef char FieldScriptManager_SizeCheck[sizeof(FieldScriptManager) == 0x464 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
FieldScriptManager *FieldScriptManager_Construct(FieldScriptManager *manager);
void FieldScriptManager_Init(FieldScriptManager *manager, void *field_context, int slot);
void FieldScriptManager_Destroy(FieldScriptManager *manager);
FieldScriptManager *FieldScriptManager_Copy(FieldScriptManager *destination,
                                            const FieldScriptManager *source);
void FieldScriptManager_StartAuxiliary(FieldScriptManager *manager, FieldScriptState *parent, int slot);
void FieldScript_BeginInteraction(FieldVmRuntime *runtime, FieldRuntimeEntity *actor,
                                  FieldRuntimeEntity *target, FieldScriptState *state);
void FieldScript_EndInteraction(FieldVmRuntime *runtime, FieldScriptState *state);
void FieldEntity_SetAnimationWait(FieldRuntimeEntity *entity, s8 enabled);
void FieldEntity_RestoreContactFlags(FieldRuntimeEntity *entity);
#ifdef __cplusplus
}
#endif

#endif

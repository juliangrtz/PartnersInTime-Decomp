#ifndef PIT_GAME_FIELD_SCRIPT_H
#define PIT_GAME_FIELD_SCRIPT_H

#include <game/script_vm.h>

typedef struct FieldEntity FieldEntity;
typedef struct FieldScriptState FieldScriptState;
typedef struct FieldVmRuntime FieldVmRuntime;

enum FieldScriptFlag {
    FIELD_SCRIPT_ENABLED = 1 << 0,
    FIELD_SCRIPT_ACTIVE = 1 << 1,
    FIELD_SCRIPT_PAUSED = 1 << 2,
    FIELD_SCRIPT_OWNER_TYPE_SHIFT = 4,
    FIELD_SCRIPT_OWNER_TYPE_MASK = 7 << FIELD_SCRIPT_OWNER_TYPE_SHIFT,
    FIELD_SCRIPT_CONTEXT_TYPE_SHIFT = 7,
    FIELD_SCRIPT_CONTEXT_TYPE_MASK = 3 << FIELD_SCRIPT_CONTEXT_TYPE_SHIFT,
    FIELD_SCRIPT_PARENT_TYPE_SHIFT = 9,
    FIELD_SCRIPT_PARENT_TYPE_MASK = 7 << FIELD_SCRIPT_PARENT_TYPE_SHIFT,
    FIELD_SCRIPT_QUEUED_PARENT_TYPE_SHIFT = 12,
    FIELD_SCRIPT_QUEUED_PARENT_TYPE_MASK =
        7 << FIELD_SCRIPT_QUEUED_PARENT_TYPE_SHIFT,
    FIELD_SCRIPT_RESULT_SHIFT = 15,
    FIELD_SCRIPT_RESULT_MASK = 3 << FIELD_SCRIPT_RESULT_SHIFT,
    FIELD_SCRIPT_INLINE_WAIT = 1 << 17,
    FIELD_SCRIPT_OWNER_SUBTYPE_SHIFT = 18,
    FIELD_SCRIPT_OWNER_SUBTYPE_MASK = 0xFF << FIELD_SCRIPT_OWNER_SUBTYPE_SHIFT
};

enum FieldScriptOwnerType {
    FIELD_SCRIPT_OWNER_PRIMARY = 1,
    FIELD_SCRIPT_OWNER_SECONDARY = 2,
    FIELD_SCRIPT_OWNER_ENTITY = 4
};

struct FieldScriptState {
    ScriptVmState vm_state;
    u32 unknown_ac;
    u32 flags;
    u8 owner_data[4];
    s8 parent_entity_id;
    s8 queued_parent_entity_id;
    u8 unknown_ba[6];
    const u16 *queued_script;
    u32 unknown_c4;
    FieldEntity *owner_entity;
};

struct FieldVmRuntime {
    ScriptVm vm;
    u8 unknown_010[0x80];
    u32 screen_selector;
    u8 *field_context;
};

typedef char FieldScriptState_SizeCheck[
    sizeof(FieldScriptState) == 0xCC ? 1 : -1];

int FieldVm_DispatchCommand(ScriptVm *vm, ScriptVmState *state,
                            ScriptVmCommand *command);

#endif

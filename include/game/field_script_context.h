#ifndef PIT_GAME_FIELD_SCRIPT_CONTEXT_H
#define PIT_GAME_FIELD_SCRIPT_CONTEXT_H

#include <game/field_script_manager.h>

/* Fields used by the script manager, with the remaining area context opaque. */
typedef struct FieldScriptContext {
    u8 unknown_0000[0x22F8];
    const u16 *area_header;
    u8 unknown_22fc[0x6C];
    u32 variables;
    u8 unknown_236c[0x8C];
    u16 screen : 1;
    u16 screen_reserved : 15;
    u8 unknown_23fa[0xE6];
    const u8 *scripts[2];
    u8 unknown_24e8[0x4F0];
    FieldEntity *entities[86];
    u8 unknown_2b30[2];
    u8 entity_count;
} FieldScriptContext;

static inline const u16 *FieldScript_Lookup(FieldScriptContext *field, unsigned resource_set, int slot)
{
    if (slot == -1)
        return 0;
    const u8 *data = field->scripts[resource_set];
    return (const u16 *)(data + *(const u16 *)(data + 2 * (slot & 0xFFFF) + 18));
}

#endif

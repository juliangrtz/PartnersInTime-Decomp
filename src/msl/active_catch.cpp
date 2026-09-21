/*
 * Recover the active catch record while searching the saved unwind state.
 * The search works on a private cursor, then updates only the original
 * exception type, object, destructor and active-catch pointer.
 *
 * These are byte-compatible search views of the unwind records. The record
 * has a sixth opaque word beyond the five-word lookup prefix; the 84-byte
 * saved block includes registers and frame metadata from unwind_internal.h.
 * Preserve scalar copies followed by the saved block's aggregate copy.
 */
extern "C" {
#include "unwind_internal.h"
}
struct MslSearchSavedState {u32 words[21];};
struct MslSearchContext {
    const u8 *type;
    void *object;
    void *destructor;
    void *active_catch;
    u32 address, stack_pointer;
    u8 *frame_pointer;
    MslSearchSavedState state;
};
struct MslSearchRecord {
    MslUnwindRecord lookup;
    u32 unknown20;
};
struct MslSearchCursor {
    MslSearchRecord record;
    MslSearchContext context;
};
struct MslActiveCatch {void *object; const u8 *type;};
typedef char MslSearchContextSize[sizeof(MslSearchContext) == 112 ? 1 : -1];
typedef char MslSearchRecordSize[sizeof(MslSearchRecord) == 24 ? 1 : -1];
typedef char MslSearchCursorSize[sizeof(MslSearchCursor) == 136 ? 1 : -1];
typedef char MslSearchSavedOffset[(u32)&((MslSearchContext *)0)->state == 28 ? 1 : -1];
extern "C" {
int func_0204803c(MslSearchCursor *);
void MSL_Terminate(void);
const u8 *MSL_DecodeSigned(const u8 *,s32 *);

MslActiveCatch *MSL_FindActiveCatch(MslSearchContext *context,const MslSearchRecord *record)
{
    s32 offset;
    MslSearchCursor cursor;
    cursor.record.lookup.function=record->lookup.function;
    cursor.record.lookup.instructions=record->lookup.instructions;
    cursor.record.lookup.handler=record->lookup.handler;
    cursor.record.lookup.table_start=record->lookup.table_start;
    cursor.record.lookup.table_end=record->lookup.table_end;
    cursor.record.unknown20=record->unknown20;
    cursor.context.type=context->type;
    cursor.context.object=context->object;
    cursor.context.destructor=context->destructor;
    cursor.context.active_catch=context->active_catch;
    cursor.context.address=context->address;
    cursor.context.stack_pointer=context->stack_pointer;
    cursor.context.frame_pointer=context->frame_pointer;
    cursor.context.state=context->state;
    int kind=MSL_GetUnwindHandlerKind(&cursor.record.lookup);
    for (;;) {
        switch (kind) {
        case 0: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
        case 9: case 10: case 11: case 12: case 15: case 16: case 17: case 18:
            break;
        case 13:
            goto found;
        default:
            MSL_Terminate();
            goto found;
        }
        kind=func_0204803c(&cursor);
    }
found:
    MSL_DecodeSigned(cursor.record.lookup.handler+1,&offset);
    MslActiveCatch *active=(MslActiveCatch *)(cursor.context.frame_pointer+offset);
    context->type=active->type;
    context->object=active->object;
    context->destructor=0;
    context->active_catch=active;
    return active;
}
}

/* Initialize a catch record in the saved frame, including pointer adjustment. */
#include <nitro.h>
/* This view covers only the 28-byte prefix consumed here; the complete
 * unwind context also contains saved registers and frame metadata. */
typedef struct CatchContext {
    const u8 *type;
    u8 *object;
    void *destructor;
    void *active_catch;
    u32 address, stack_pointer;
    u8 *frame_pointer;
} CatchContext;
typedef struct CatchRecord {
    u8 *object;
    const u8 *type;
    void *destructor;
    u8 *adjusted_object;
    u8 *pointer_value;
} CatchRecord;

typedef char CatchContextPrefixSize[sizeof(CatchContext) == 28 ? 1 : -1];
typedef char CatchRecordSize[sizeof(CatchRecord) == 20 ? 1 : -1];
/* A pointer exception stores its adjusted pointer in the record itself.
 * An object exception instead points directly into the thrown object. */
void MSL_SetupCatchRecord(CatchContext *context, int frame_offset, int adjustment)
{
    CatchRecord *record = (CatchRecord *)(context->frame_pointer + frame_offset);
    record->object = context->object;
    record->type = context->type;
    record->destructor = context->destructor;
    if (*context->type == '*') {
        record->adjusted_object = (u8 *)&record->pointer_value;
        record->pointer_value = *(u8 **)context->object + adjustment;
    } else {
        record->adjusted_object = context->object + adjustment;
    }
}

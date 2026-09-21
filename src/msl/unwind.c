/* Decode frame headers and restore saved ARM registers during exception unwinding. */
#include "unwind_internal.h"

/* Thirty native entries end at the beginning of .init. The table stays native. */
extern const MslExceptionEntry gMslExceptionTable[30];

u32 MSL_RestoreFrameRegisters(MslUnwindContext *context)
{
    u32 size = context->frame_size;
    u32 *end = (u32 *)(context->frame_pointer + size - (context->stack_header ? 16 : 0));
    int index;
    for (index = 15; index >= 0; --index) {
        if (context->saved_register_mask & (1 << index))
            context->registers[index] = *--end;
    }
    context->stack_pointer = context->frame_pointer + size;
    return context->registers[14];
}

void MSL_DecodeFrameHeader(MslUnwindContext *context, const MslUnwindRecord *record)
{
    const u8 *input = record->instructions;
    int flags = input[0];
    int has_frame_pointer = flags & 64;
    int frame_uses_r7;
    context->has_frame_pointer = has_frame_pointer ? 1 : 0;
    context->stack_header = (flags & 32) ? 1 : 0;
    frame_uses_r7 = flags & 128;
    context->frame_uses_r7 = frame_uses_r7 ? 1 : 0;
    context->saved_register_mask = input[1] << 4;
    context->saved_register_mask |= 0x4000;
    input = MSL_DecodeUnsigned(input + 2, &context->frame_size);
    if (has_frame_pointer)
        input = MSL_DecodeUnsigned(input, &context->stack_adjustment);
    if (has_frame_pointer) {
        if (frame_uses_r7)
            context->frame_pointer = context->registers[7];
        else
            context->frame_pointer = context->registers[11];
    } else {
        context->frame_pointer = context->stack_pointer;
    }
}

int MSL_GetExceptionTable(MslUnwindRecord *record, u32 address)
{
    /* The native runtime has one table; address does not select a module. */
    record->table_start = gMslExceptionTable;
    record->table_end = gMslExceptionTable + 30;
    return 1;
}

const u8 *MSL_SkipFrameHeader(const u8 *input)
{
    int flags = input[0];
    u32 ignored;
    input = MSL_DecodeUnsigned(input + 2, &ignored);
    if (flags & 64)
        input = MSL_DecodeUnsigned(input, &ignored);
    return input;
}

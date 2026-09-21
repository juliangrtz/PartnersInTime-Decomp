/*
 * Static object lifetime (ARM9 resident, 0x020483D8-0x02048614).
 *
 * Runs the global constructors at startup, tears the destructor chain down at
 * exit, and finds the exception entry for an address.
 */

#include "unwind_internal.h"

typedef struct MslDestructorNode {
    struct MslDestructorNode *next;
    void (*destructor)(void *object, int mode);
    void *object;
} MslDestructorNode;

extern MslDestructorNode *__global_destructor_chain;
extern void (*data_0204ff6c[])(void);

void MSL_RunGlobalConstructors(void)
{
    void (**constructor)(void) = data_0204ff6c;
    for (; constructor && *constructor; ++constructor) {
        (*constructor)();
    }
}

void MSL_DestroyGlobalChain(void)
{
    MslDestructorNode *node;
    while ((node = __global_destructor_chain) != 0) {
        __global_destructor_chain = node->next;
        node->destructor(node->object, -1);
    }
}

const MslExceptionEntry *MSL_FindExceptionEntry(const MslExceptionEntry *table, int count, u32 address)
{
    int first;
    int last = count - 1;
    first = 0;
    while (first <= last) {
        int middle = (first + last) >> 1;
        u32 start = table[middle].start;
        if (address < start) {
            last = middle - 1;
        } else {
            if (address > start + (table[middle].length_and_flags & ~1)) {
                first = middle + 1;
            } else {
                return &table[middle];
            }
        }
    }
    return 0;
}

/* Handler intervals are delta-coded; both ends use the native inclusive tests. */
void MSL_LookupUnwindRecord(u32 address, MslUnwindRecord *record)
{
    const MslExceptionEntry *entry;
    const u8 *input;
    u32 offset, cursor;
    u32 skip, length, handler_offset;
    record->instructions = 0;
    record->handler = 0;
    if (!MSL_GetExceptionTable(record, address))
        return;
    entry = MSL_FindExceptionEntry(record->table_start,
        record->table_end - record->table_start, address);
    if (!entry)
        return;
    if (entry->length_and_flags & 1)
        record->instructions = (const u8 *)&entry->information;
    else
        record->instructions = (const u8 *)entry->information;
    record->function = entry->start;
    offset = address - entry->start;
    input = MSL_SkipFrameHeader(record->instructions);
    cursor = 0;
    for (;;) {
        u32 start;
        input = MSL_DecodeUnsigned(input, &skip);
        if (!skip)
            return;
        input = MSL_DecodeUnsigned(input, &length);
        input = MSL_DecodeUnsigned(input, &handler_offset);
        start = cursor + skip;
        if (offset < start)
            return;
        cursor = start + length;
        if (offset <= cursor) {
            record->handler = record->instructions + handler_offset;
            return;
        }
    }
}

/* The low five bits select the action; bit 7 marks a frame boundary. */
u8 MSL_GetUnwindHandlerKind(const MslUnwindRecord *record)
{
    return record->handler ? *record->handler & 31 : 0;
}

#ifndef PIT_MSL_UNWIND_INTERNAL_H
#define PIT_MSL_UNWIND_INTERNAL_H

/* Frame descriptions and saved ARM registers used by the MSL exception runtime. */
#include <nitro.h>

typedef struct MslExceptionEntry {
    u32 start, length_and_flags, information;
} MslExceptionEntry;

typedef struct MslUnwindContext {
    u8 unknown00[20];
    u32 stack_pointer;
    u32 frame_pointer;
    u32 registers[16];
    u32 original_stack_pointer;
    u32 frame_size;
    u32 stack_adjustment;
    u16 saved_register_mask;
    u8 stack_header, has_frame_pointer, frame_uses_r7;
    u8 padding109[3];
} MslUnwindContext;

typedef struct MslUnwindRecord {
    u32 function;
    const u8 *instructions;
    const u8 *handler;
    const MslExceptionEntry *table_start, *table_end;
} MslUnwindRecord;

typedef char MslUnwindContextSize[sizeof(MslUnwindContext) == 112 ? 1 : -1];
typedef char MslUnwindRegistersOffset[(u32)&((MslUnwindContext *)0)->registers == 28 ? 1 : -1];
typedef char MslUnwindMaskOffset[(u32)&((MslUnwindContext *)0)->saved_register_mask == 104 ? 1 : -1];
typedef char MslUnwindRecordSize[sizeof(MslUnwindRecord) == 20 ? 1 : -1];

const u8 *MSL_DecodeUnsigned(const u8 *input, u32 *value);
const u8 *MSL_SkipFrameHeader(const u8 *input);
int MSL_GetExceptionTable(MslUnwindRecord *record, u32 address);
void MSL_DecodeFrameHeader(MslUnwindContext *context, const MslUnwindRecord *record);
u32 MSL_RestoreFrameRegisters(MslUnwindContext *context);
void MSL_LookupUnwindRecord(u32 address, MslUnwindRecord *record);
const MslExceptionEntry *MSL_FindExceptionEntry(const MslExceptionEntry *table, int count, u32 address);

#endif

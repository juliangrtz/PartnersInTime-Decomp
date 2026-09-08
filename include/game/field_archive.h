#ifndef PIT_GAME_FIELD_ARCHIVE_H
#define PIT_GAME_FIELD_ARCHIVE_H

#include <game/archive_io.h>

/* The high ID byte selects a resource table; the low 24 bits select an
 * eight-byte record. Localized entries occupy consecutive language records. */
typedef struct FieldArchiveRecord {
    u8 data[7];
    u8 localized : 1;
    u8 reserved : 7;
} FieldArchiveRecord;

typedef struct FieldArchiveRequest {
    ArchiveReadRequest read;
    u32 unknown_28;
} FieldArchiveRequest;

typedef struct FieldArchive {
    ArchiveCompressedIO base;
    u8 unknown_340[0x100];
    u16 load_phase;
    u16 reserved_442;
    u32 descriptor_sizes[7];
    u8 *descriptors[7];
    FieldArchiveRequest *pending_reads;
    u32 *resource_data[3];
    FieldArchiveRecord *first_records[3];
    u32 first_count[3];
    FieldArchiveRecord *second_records[3];
    u32 second_count[3];
} FieldArchive;

typedef char FieldArchiveRecord_SizeCheck[sizeof(FieldArchiveRecord) == 8 ? 1 : -1];
typedef char FieldArchiveRequest_SizeCheck[sizeof(FieldArchiveRequest) == 44 ? 1 : -1];
typedef char FieldArchive_SizeCheck[sizeof(FieldArchive) == 0x4BC ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
FieldArchive *FieldArchive_Init(FieldArchive *archive, u32 priority, u32 unused, void *argument, int dma);
FieldArchive *FieldArchive_Destroy(FieldArchive *archive);
FieldArchive *FieldArchive_Delete(FieldArchive *archive);
const char *FieldArchive_GetDescriptor(FieldArchive *archive, int index);
FieldArchiveRecord *FieldArchive_GetPrimaryRecord(FieldArchive *archive, u32 id);
FieldArchiveRecord *FieldArchive_GetSecondaryRecord(FieldArchive *archive, u32 id);
int FieldArchive_GetArchiveIndex(FieldArchive *archive, u32 id);
#ifdef __cplusplus
}
#endif

#endif

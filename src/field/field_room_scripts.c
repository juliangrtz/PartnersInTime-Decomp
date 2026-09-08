#include <game/field_resources.h>
#include <game/battle_archive.h>
#include <game/heap.h>
extern void func_0202cbd4(void *, int, u32);
typedef struct FieldRoomArchiveRecord {
    int entry;
    u8 unknown_04[16];
} FieldRoomArchiveRecord;
extern FieldRoomArchiveRecord data_02051da0[];
void FieldResources_LoadRoomScripts(void *field_context)
{
    FieldResourceContext *field = (FieldResourceContext *)field_context;
    const char *descriptor;
    field->reads = GameHeap_NewArray(4 * sizeof(FieldArchiveRequest), field->heaps.temporary, 0, 1);
    func_0202cbd4(field->reads, 0, 4 * sizeof(FieldArchiveRequest));
    field->reads[0].read.entry = 3 * field->room_id;
    field->reads[0].read.limit = BattleArchive_GetEntrySize(field->archive, field->archive->descriptors[0],
                                                            field->reads[0].read.entry);
    field->reads[0].read.destination = GameHeap_NewArray(field->reads[0].read.limit, field->heaps.main, 0, 1);
    field->scripts[0] = field->reads[0].read.destination;
    descriptor = FieldArchive_GetDescriptor(field->archive, 0);
    BattleArchive_ReadAsync(field->archive, &field->reads[0], 0, field->archive->descriptors[0], descriptor,
                            0x8000);

    field->reads[1].read.entry = 3 * field->room_id + 1;
    field->reads[1].read.limit = BattleArchive_GetEntrySize(field->archive, field->archive->descriptors[0],
                                                            field->reads[1].read.entry);
    if (field->reads[1].read.limit) {
        field->reads[1].read.destination =
            GameHeap_NewArray(field->reads[1].read.limit, field->heaps.temporary, 0, 1);
        field->room_extra = field->reads[1].read.destination;
        BattleArchive_ReadAsync(field->archive, &field->reads[1], 0, field->archive->descriptors[0], 0,
                                0x8000);
    } else
        field->room_extra = 0;

    field->reads[2].read.entry = 3 * field->room_id + 2;
    field->reads[2].read.limit = BattleArchive_GetEntrySize(field->archive, field->archive->descriptors[0],
                                                            field->reads[2].read.entry);
    if (field->reads[2].read.limit) {
        field->reads[2].read.destination =
            GameHeap_NewArray(field->reads[2].read.limit, field->heaps.main, 0, 1);
        field->scripts[1] = field->reads[2].read.destination;
        BattleArchive_ReadAsync(field->archive, &field->reads[2], 0, field->archive->descriptors[0], 0,
                                0x8000);
    } else
        field->scripts[1] = 0;

    if (data_02051da0[field->room_id].entry != -1) {
        field->reads[3].read.entry = data_02051da0[field->room_id].entry;
        field->reads[3].read.limit = BattleArchive_GetEntrySize(
            field->archive, field->archive->descriptors[5], field->reads[3].read.entry);
        field->reads[3].read.destination =
            GameHeap_NewArray(field->reads[3].read.limit, field->heaps.main, 0, 1);
        field->room_resource = field->reads[3].read.destination;
        descriptor = FieldArchive_GetDescriptor(field->archive, 5);
        BattleArchive_ReadAsync(field->archive, &field->reads[3], 0, field->archive->descriptors[5],
                                descriptor, 0x8000);
    } else
        field->room_resource = 0;
}

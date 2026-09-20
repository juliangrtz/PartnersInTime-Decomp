/* Allocate the two room resource sets and their archive request arrays.
 * Only the first five special-resource indices are reset. Each primary resource
 * needs two compressed requests; each secondary/alternate needs two ordinary
 * requests. Cache both primary counts before allocating the request arrays.
 */
#include <game/field_resources.h>
extern "C" {
#include <game/heap.h>
extern void func_0202cbd4(void *, int, u32);
void FieldResources_AllocateRoomBuffers(FieldResourceContext *field)
{
    int set;
    int alternate_primary_count, primary_count;
    u32 read_bytes, compressed_bytes;
    field->special_resources[0] = -1;
    field->special_resources[1] = -1;
    field->special_resources[2] = -1;
    field->special_resources[3] = -1;
    field->special_resources[4] = -1;
    for (set = 0; set < 2; ++set) {
        if (field->primary_counts[set] > 0) {
            field->primary[set] = (FieldPrimaryResource *)GameHeap_NewArray(
                sizeof(FieldPrimaryResource) * field->primary_counts[set], field->heaps.main, 0, 1);
            func_0202cbd4(field->primary[set], 0,
                          sizeof(FieldPrimaryResource) * field->primary_counts[set]);
        } else
            field->primary[set] = 0;
        if (field->secondary_counts[set] > 0) {
            field->secondary[set] = (FieldSecondaryResource *)GameHeap_NewArray(
                sizeof(FieldSecondaryResource) * field->secondary_counts[set], field->heaps.main, 0,
                1);
            func_0202cbd4(field->secondary[set], 0,
                          sizeof(FieldSecondaryResource) * field->secondary_counts[set]);
        } else
            field->secondary[set] = 0;
        if (field->alternate_counts[set] > 0) {
            field->alternate[set] = (FieldSecondaryResource *)GameHeap_NewArray(
                sizeof(FieldSecondaryResource) * field->alternate_counts[set], field->heaps.main, 0,
                1);
            func_0202cbd4(field->alternate[set], 0,
                          sizeof(FieldSecondaryResource) * field->alternate_counts[set]);
        } else
            field->alternate[set] = 0;
    }
    primary_count = field->primary_counts[0];
    alternate_primary_count = field->primary_counts[1];
    read_bytes =
        sizeof(FieldArchiveRequest) *
        ((primary_count + 2 * (field->secondary_counts[0] + field->alternate_counts[0])) +
         alternate_primary_count + 2 * (field->secondary_counts[1] + field->alternate_counts[1]));
    field->reads =
        (FieldArchiveRequest *)GameHeap_NewArray(read_bytes, field->heaps.temporary, 0, 1);
    func_0202cbd4(field->reads, 0, read_bytes);
    compressed_bytes =
        (primary_count + alternate_primary_count) * 2 * sizeof(ArchiveCompressedRequest);
    field->compressed_reads = (ArchiveCompressedRequest *)GameHeap_NewArray(
        compressed_bytes, field->heaps.temporary, 0, 1);
    func_0202cbd4(field->compressed_reads, 0, compressed_bytes);
    field->read_cursor = field->reads;
    field->compressed_cursor = field->compressed_reads;
    field->read_count = 0;
    field->compressed_count = 0;
}
}

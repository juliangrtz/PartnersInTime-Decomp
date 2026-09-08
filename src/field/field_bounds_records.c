#include <game/field_bounds_records.h>

void FieldArea_ExpandBoundaryRecords(void *unused, const FieldBoundaryRecord *record,
                                     FieldBoundaryRuntime *bounds)
{
    if (record) {
        for (;;) {
            bounds->flags.active = 0;
            bounds->flags.last = 0;
            bounds->flags.index = record->flags.index;
            bounds->flags.mode = (record->flags.modes >> 0) & 3;
            bounds->planes[0].mode = (record->flags.modes >> 2) & 3;
            bounds->planes[1].mode = (record->flags.modes >> 4) & 3;
            bounds->planes[2].mode = (record->flags.modes >> 6) & 3;
            bounds->planes[3].mode = (record->flags.modes >> 8) & 3;
            bounds->flags.enabled = (record->flags.enabled >> 0) & 1;
            bounds->planes[0].enabled = (record->flags.enabled >> 1) & 1;
            bounds->planes[1].enabled = (record->flags.enabled >> 2) & 1;
            bounds->planes[2].enabled = (record->flags.enabled >> 3) & 1;
            bounds->planes[3].enabled = (record->flags.enabled >> 4) & 1;
            bounds->coordinates[0] = record->coordinates[0] << 12;
            bounds->coordinates[1] = record->coordinates[1] << 12;
            bounds->coordinates[2] = record->coordinates[2] << 12;
            bounds->coordinates[3] = record->coordinates[3] << 12;
            bounds->coordinates[4] = record->coordinates[4] << 12;
            bounds->planes[0].value = record->planes[0] << 12;
            bounds->planes[1].value = record->planes[1] << 12;
            bounds->planes[2].value = record->planes[2] << 12;
            bounds->planes[3].value = record->planes[3] << 12;
            ++bounds;
            if (record->flags.last)
                break;
            ++record;
        }
    }
    bounds->flags.last = 1;
}
void FieldArea_ExpandPairedBounds(void *unused, const FieldPairedBoundsRecord *record,
                                  FieldPairedBoundsRuntime *bounds)
{
    if (record) {
        for (;;) {
            bounds->flags.active = 0;
            bounds->flags.last = 0;
            bounds->flags.unknown_02 = record->flags.unknown_01;
            bounds->flags.mode = (u16)(record->flags.mode << 1);
            bounds->flags.unknown_06 = record->flags.unknown_04;
            bounds->flags.unknown_07_08 = record->flags.unknown_05_06;
            bounds->id = record->id;
            bounds->bounds[0].min_x = record->bounds[0].x << 12;
            bounds->bounds[0].max_x = bounds->bounds[0].min_x + (record->bounds[0].width << 12);
            bounds->bounds[0].min_y = record->bounds[0].y << 12;
            bounds->bounds[0].max_y = bounds->bounds[0].min_y + (record->bounds[0].height << 12);
            bounds->bounds[0].z = record->bounds[0].z << 12;
            bounds->bounds[1].min_x = record->bounds[1].x << 12;
            bounds->bounds[1].max_x = bounds->bounds[1].min_x + (record->bounds[1].width << 12);
            bounds->bounds[1].min_y = record->bounds[1].y << 12;
            bounds->bounds[1].max_y = bounds->bounds[1].min_y + (record->bounds[1].height << 12);
            bounds->bounds[1].z = record->bounds[1].z << 12;
            ++bounds;
            if (record->flags.last)
                break;
            ++record;
        }
    }
    bounds->flags.last = 1;
}

#include <game/field_area.h>
extern "C" {
#include <game/heap.h>
extern const u8 data_ov000_020bfb94[];
extern void func_ov000_020bdc88(FieldNavigationSurface *, fx32, fx32, fx32 *, fx32 *, int);

static inline void ResetEdgeMasks(FieldNavigationSurface *surface, int count)
{
    if (count == 3) {
        surface->bits.edge_mask_a = 7;
        surface->bits.edge_mask_b = 7;
        surface->bits.edge_mask_c = !surface->bits.unknown_02 ? 7 : 0;
        surface->bits.edge_mask_d = 7;
    } else {
        surface->bits.edge_mask_a = 15;
        surface->bits.edge_mask_b = 15;
        surface->bits.edge_mask_c = !surface->bits.unknown_02 ? 15 : 0;
        surface->bits.edge_mask_d = 15;
    }
}

void FieldArea_AllocateNavigationRecords(FieldAreaContext *area)
{
    area->navigation_resource = area->navigation_changes->get_navigation();
    if (area->navigation_resource) {
        area->navigation_surface_count = area->navigation_resource->surface_count;
        area->boundary_count = area->navigation_resource->boundary_count;
        if (!area->navigation_surface_count) {
            area->navigation_records = 0;
            area->navigation_surfaces = 0;
        } else {
            area->navigation_records = area->navigation_resource->records;
            area->navigation_surfaces = (FieldNavigationSurface *)GameHeap_NewArray(
                sizeof(FieldNavigationSurface) * (area->navigation_surface_count + 1), area->heaps.main, 0,
                1);
            area->navigation_surfaces->bits.end = 1;
        }
        if (!area->boundary_count) {
            area->boundary_records = 0;
            area->boundaries = 0;
        } else {
            area->boundary_records = (const FieldBoundaryRecord *)&area->navigation_resource
                                         ->records[area->navigation_surface_count];
            area->boundaries = (FieldBoundaryRuntime *)GameHeap_NewArray(
                sizeof(FieldBoundaryRuntime) * (area->boundary_count + 1), area->heaps.main, 0, 1);
            area->boundaries->flags.last = 1;
        }
    } else {
        area->navigation_surface_count = 0;
        area->boundary_count = 0;
        area->navigation_records = 0;
        area->navigation_surfaces = 0;
        area->boundary_records = 0;
        area->boundaries = 0;
    }
    area->paired_records = area->navigation_changes->get_paired_bounds();
    if (area->paired_records) {
        const FieldPairedBoundsRecord *record = area->paired_records;
        area->paired_count = 0;
        for (;;) {
            ++area->paired_count;
            if (record->flags.last)
                break;
            ++record;
        }
        area->paired_bounds = (FieldPairedBoundsRuntime *)GameHeap_NewArray(
            sizeof(FieldPairedBoundsRuntime) * (area->paired_count + 1), area->heaps.main, 0, 1);
        area->paired_bounds->flags.last = 1;
    } else {
        area->paired_count = 0;
        area->paired_bounds = 0;
    }
}

void FieldArea_ExpandNavigationRecords(FieldAreaContext *area, const FieldNavigationRecord *record,
                                       FieldNavigationSurface *surface)
{
    if (record) {
        for (;;) {
            int count = data_ov000_020bfb94[record->bits.type];
            int i;
            surface->bits.active = 0;
            surface->bits.end = 0;
            surface->bits.unknown_02 = record->bits.unknown_01;
            surface->bits.type = record->bits.type;
            surface->bits.vertex_count = count;
            ResetEdgeMasks(surface, count);
            surface->attributes = record->attributes;
            surface->index = record->bits.index;
            for (i = 0; i < count; i++) {
                surface->vertices[i].x = record->vertices[i].x << 12;
                surface->vertices[i].y = record->vertices[i].y << 12;
                surface->vertices[i].z = record->vertices[i].z << 12;
                surface->vertices[i].lower_z = record->vertices[i].lower_z << 12;
            }
            FieldArea_UpdateNavigationBounds(area, surface);
            if (!surface->bits.type) {
                fx32 x = (surface->vertices[2].x + (surface->vertices[0].x + surface->vertices[1].x)) / 3;
                fx32 y = (surface->vertices[2].y + (surface->vertices[0].y + surface->vertices[1].y)) / 3;
                fx32 center_z, center_lower_z, x_z, x_lower_z, y_z, y_lower_z;
                fx32 dx, dy;
                func_ov000_020bdc88(surface, x, y, &center_z, &center_lower_z, 1);
                func_ov000_020bdc88(surface, x + 4096, y, &x_z, &x_lower_z, 1);
                func_ov000_020bdc88(surface, x, y + 4096, &y_z, &y_lower_z, 1);
                dy = center_z - y_z;
                if (dy < 0)
                    dy = -dy;
                dx = center_z - x_z;
                if (dx < 0)
                    dx = -dx;
                surface->bits.slope_axis = dx > dy;
            }
            ++surface;
            if (record->bits.last)
                break;
            ++record;
        }
        surface->bits.end = 1;
    } else
        surface->bits.end = 1;
}

void FieldArea_UpdateNavigationBounds(FieldAreaContext *area, FieldNavigationSurface *surface)
{
    u32 i;
    surface->min_x = 0x7FFFFFFF;
    surface->max_x = -1;
    surface->min_y = 0x7FFFFFFF;
    surface->max_y = -1;
    for (i = 0; i < surface->bits.vertex_count; i++) {
        if (surface->min_x > surface->vertices[i].x)
            surface->min_x = surface->vertices[i].x;
        if (surface->max_x < surface->vertices[i].x)
            surface->max_x = surface->vertices[i].x;
        if (surface->min_y > surface->vertices[i].y)
            surface->min_y = surface->vertices[i].y;
        if (surface->max_y < surface->vertices[i].y)
            surface->max_y = surface->vertices[i].y;
    }
}
}

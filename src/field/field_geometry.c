#include <game/field_geometry.h>

int FieldGeometry_GetOrientation(int x0, int y0, int x1, int y1, int x2, int y2)
{
    /* Round each Q12 product before taking the determinant's sign. */
    int cross = (int)(((s64)(x1 - x0) * (y2 - y1) + 2048) >> 12) -
                (int)(((s64)(y1 - y0) * (x2 - x1) + 2048) >> 12);
    if (cross > 0) return 1;
    return -(cross < 0);
}

const void *FieldResource_GetSection(const u32 *resource, int section)
{
    const void *result = 0;
    if (resource && FieldResource_GetSectionSize(resource, section))
        result = (const u8 *)resource + resource[section];
    return result;
}

u32 FieldResource_GetSectionSize(const u32 *resource, int section)
{
    u32 size = 0;
    if (resource) size = (resource + section)[1] - resource[section];
    return size;
}

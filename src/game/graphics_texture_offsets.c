#include <game/graphics_resource.h>
extern const u8 data_020499bc[8];
extern const u16 data_02049940[12];

static inline int GameGraphics_ObjectBytes(const GameGraphicsObject *object)
{
    return data_02049940[(object->shape << 2) | object->size];
}

void GameGraphics_BuildGroupTextureOffsets(u16 *output, const GameGraphicsResource *resource, int group)
{
    int shift;
    int group_end;
    const GameGraphicsRange *range;
    const GameGraphicsObject *objects;
    range = GameGraphics_GetSection(resource, 3);
    objects = GameGraphics_GetSection(resource, 4);
    shift = !resource->flags.bits.color256;
    if (group == -1) {
        group_end = resource->group_count;
        group = 0;
    } else {
        group_end = group + 1;
        range += group;
    }
    for (; group < group_end; group = (s16)(group + 1), ++range) {
        u32 offset;
        int index;
        int previous;
        const GameGraphicsObject *object;
        int first;
        int end;
        int last_image;
        first = range->first;
        end = range->end;
        last_image = -1;
        object = objects + first;
        offset = 0;
        for (index = first; index < end; ++output) {
            int image = object->image;
            int reused = 0;
            if (last_image >= image) {
                const GameGraphicsObject *earlier;
                reused = 1;
                earlier = object;
                for (previous = index - 1; previous >= first; --previous) {
                    --earlier;
                    if (earlier->image == image) {
                        reused = 2;
                        *output = *(output - (index - previous));
                        break;
                    }
                }
            }
            if (reused < 2) {
                *output = offset >> 3;
                if (!reused) last_image = image;
                offset += GameGraphics_ObjectBytes(object) >> shift;
            }
            ++index;
            ++object;
        }
    }
}

void GameGraphics_BuildTextureOffsets(u16 *output, const GameGraphicsResource *resource)
{
    int previous;
    int image;
    u32 offset = 0;
    int last_image = -1;
    const GameGraphicsObject *object;
    int shift;
    int index;
    int count;
    GameGraphics_GetSection(resource, 3);
    object = GameGraphics_GetSection(resource, 4);
    shift = !resource->flags.bits.color256;
    count = resource->object_count;
    for (index = 0; index < count; ++index, ++object, ++output) {
        image = object->image;
        if (last_image >= image) {
            const GameGraphicsObject *earlier;
            earlier = object;
            previous = index - 1;
            while (previous >= 0) {
                --earlier;
                if (earlier->image == image) {
                    *output = *(output - (index - previous));
                    break;
                }
                --previous;
            }
        } else {
            *output = offset >> 3;
            last_image = (s16)image;
            offset += GameGraphics_ObjectBytes(object) >> shift;
        }
    }
}

u8 GameGraphics_GetTextureFormat(const GameGraphicsResource *resource)
{
    return data_020499bc[resource->flags.bits.texture_format];
}

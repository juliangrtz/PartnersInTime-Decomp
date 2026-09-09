#include <game/graphics_resource.h>
#include <game/sprite_output.h>
extern const u16 data_02049940[12];

static inline u32 AlignTextureOffset(u32 value, u32 mask)
{
    return (value + mask) & ~mask;
}

static inline int GameGraphics_ObjectBytes(const GameGraphicsObject *object)
{
    return data_02049940[(object->shape << 2) | object->size];
}

void GameGraphics_BuildScreenTextureOffsets(int screen, u16 *output, const GameGraphicsResource *resource)
{
    int boundary_mask = GameSprite_ObjBoundary(screen) - 1;
    int boundary_shift = GameSprite_ObjBoundaryShift(screen) + 5;
    int previous;
    u32 offset = 0;
    int last_image = -1;
    const GameGraphicsObject *object;
    int shift;
    int index;
    int count;
    GameGraphics_GetSection(resource, 3);
    object = GameGraphics_GetSection(resource, 4);
    shift = resource->flags.bits.color256 ^ 1;
    count = resource->object_count;
    if (resource->flags.bits.alternate_boundary == GameSprite_ObjBoundaryShift(screen) + 5) {
        for (index = 0; index < count; ++index, ++object, ++output)
            *output = object->image;
    } else {
        for (index = 0; index < count; ++index, ++object, ++output) {
            int image = object->image;
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
                *output = offset >> boundary_shift;
                last_image = (s16)image;
                offset =
                    AlignTextureOffset(offset + (GameGraphics_ObjectBytes(object) >> shift), boundary_mask);
            }
        }
    }
}

void GameGraphics_InitOffsetDecoder(GameGraphicsOffsetDecoder *state, int screen, int texture, int alternate,
                                    u16 *output, const GameGraphicsResource *resource)
{
    const GameGraphicsRange *groups;
    const GameGraphicsObject *objects;
    state->output = output;
    state->resource = resource;
    groups = GameGraphics_GetSection(resource, 3);
    objects = GameGraphics_GetSection(resource, 4);
    state->groups = groups;
    state->objects = objects;
    state->group = 0;
    state->object = groups->first;
    state->last_image = -1;
    state->offset = 0;
    state->output_count = 0;
    if (!screen && texture) {
        state->options.halves.boundary_mask = 7;
        state->options.bits.boundary_shift = 3;
    } else {
        state->options.halves.boundary_mask = GameSprite_ObjBoundary(screen) - 1;
        state->options.bits.boundary_shift = GameSprite_ObjBoundaryShift(screen) + 5;
    }
    state->options.bits.alternate = alternate;
}

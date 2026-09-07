#include <game/battle_scene.h>
#include <game/affine.h>
extern "C" {
#include <game/sprite_output.h>
}

/* A packed transform word holds three ten-bit components; the control halfword
   picks which one the caller wants and how far it is shifted. */
struct BattleTransformPack {
    union {
        u16 raw;
        struct {
            u16 unknown_00_03 : 4, component : 3, unknown_07_13 : 7, wide : 1,
                unknown_15 : 1;
        } bits;
    } control;
    u16 unknown_02;
    union {
        u32 raw;
        struct { u32 x : 10, y : 10, z : 10, reserved : 2; } bits;
    } packed[2];
};

/* A row of the transform table: a 4x4 matrix whose last column is respaced. */
struct BattleTransformRow {
    s16 matrix[4][4];
};

typedef void (*BattleModelRenderProc)(void);

extern "C" {

extern u8 data_02060340[2][1024];

extern void func_0200c3b0(void), func_0200c3b8(void), func_0200c3c0(void);
extern void func_0200c3d8(void *argument);

u32 func_02009474(const BattleTransformPack *pack, int word, int component);

/* Adds a palette offset to every non-transparent pixel index. */
void func_02009644(const u8 *source, u8 *destination, u32 count, int palette)
{
    u32 index;
    if (palette == 0) return;
    if (destination == 0) destination = (u8 *)source;
    for (index = 0; index < count;) {
        u8 pixel = *source;
        ++index;
        ++source;
        if (pixel != 0) *destination = pixel + (u8)(palette << 4);
        ++destination;
    }
}

u8 func_02009630(BattleModel *model)
{
    return model->flag_bits.unknown_00_01;
}

void func_02009598(BattleTransformRow *rows, u8 first, u8 last)
{
    BattleTransformRow *row = &rows[first];
    AffineMatrixQ8 source;
    AffineMatrixQ8 destination;
    for (; first < last; ++first) {
        source.values[0] = row->matrix[0][3];
        source.values[1] = row->matrix[1][3];
        source.values[2] = row->matrix[2][3];
        source.values[3] = row->matrix[3][3];
        GameAffine_InvertQ8(&source, &destination);
        row->matrix[0][3] = destination.values[0];
        row->matrix[1][3] = destination.values[1];
        row->matrix[2][3] = destination.values[2];
        row->matrix[3][3] = destination.values[3];
        ++row;
    }
}

void func_02009558(BattleModel *model, int enabled)
{
    if (model->flag_bits.no_sort_key) return;
    if (model->flag_bits.unknown_16_18 == 3) return;
    if (enabled) model->property_028 |= 0x10;
    else model->property_028 &= ~0x10;
}

int func_02009540(BattleModel *model)
{
    return (model->property_028 & 0x10) == 0x10;
}

void func_02009518(BattleModel *model, int enabled)
{
    if (enabled) model->property_02c[0x13] |= 8;
    else model->property_02c[0x13] &= ~8;
}

int func_020094fc(BattleModel *model)
{
    return (model->property_02c[0x13] & 8) == 8;
}

u32 func_02009498(const BattleTransformPack *pack)
{
    u32 component = pack->control.bits.component;
    if (component >= 5) {
        u32 index = component - 5;
        return ((pack->packed[1].raw >> (index * 10)) & 0x3ff) << (index + 5);
    }
    if (pack->control.bits.wide) return pack->packed[1].bits.y << 6;
    return pack->packed[1].bits.x << 5;
}

u32 func_02009474(const BattleTransformPack *pack, int word, int component)
{
    u32 packed = word == 1 ? pack->packed[1].raw : pack->packed[0].raw;
    return (packed >> (component * 10)) & 0x3ff;
}

void func_0200940c(BattleModel *model, s16 value)
{
    const BattleModelFrameEntry *frame =
        &model->frames[model->property_056 +
                       model->animation_data[model->animation_id].start_frame];
    if ((s32)(frame->value.bits.offset << 8) < model->anchor_offset) {
        model->anchor_offset = model->anchor_offset - model->effect_scale;
        model->anchor_offset = model->anchor_offset + value;
    }
    model->effect_scale = value;
}

void func_020093b4(BattleModel *model, int enabled)
{
    if (!model->flag_bits.animation_active && enabled) model->unknown_48(1);
    model->flag_bits.animation_active = enabled;
}

/* Carries the animation state of one model over to another. */
void func_02009360(BattleModel *source, BattleModel *destination)
{
    destination->set_animation((u8)source->get_animation_id(),
                               source->property_056);
    destination->anchor_offset = source->anchor_offset;
    destination->effect_scale = source->effect_scale;
}

BattleModelRenderProc func_0200930c(BattleModel *model, void *override)
{
    if (override == 0) {
        return model->flag_bits.no_sort_key == 0 ? func_0200c3b8 : func_0200c3b0;
    }
    func_0200c3d8(override);
    return func_0200c3c0;
}

/* Draws every model on one engine's render list and records the object range
   each of them claimed. */
void func_02009254(int screen, u8 object_count, u8 affine_count, void *buffer)
{
    BattleModel *model = gModelRenderList[screen];
    u8 objects = object_count;
    u8 affines = affine_count;
    if (buffer == 0) buffer = data_02060340[screen];
    while (model != 0) {
        model->draw(buffer, &objects, &affines);
        GameOam_AddGroup(screen, model->get_sort_key(), objects - object_count,
                         affines - affine_count);
        object_count = objects;
        affine_count = affines;
        model = model->render_next;
    }
}

u16 func_02009224(BattleModel *model, s16 animation_id)
{
    if (animation_id == -1) animation_id = model->animation_id;
    return model->animation_data[animation_id].end_frame -
           model->animation_data[animation_id].start_frame;
}

s32 func_0200921c(BattleModel *model)
{
    return model->render_anchor_z;
}

u16 func_02009210(BattleModel *model)
{
    return model->property_038[6];
}

u16 func_02009208(BattleModel *model)
{
    return model->animation_id;
}

}

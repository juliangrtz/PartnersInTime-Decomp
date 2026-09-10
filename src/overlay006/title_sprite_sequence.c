#include "title_sprite_render_internal.h"
#include <game/heap.h>
extern const TitleSpriteLayout data_ov006_0207b2a4[], data_ov006_0207b2c4[], data_ov006_0207b2d4[];
extern const s16 FX_SinCosTable_[];
int _s32_div_f(int, int);
int func_02035818(void);
void func_02035c00(int);
int func_ov006_02075120(TitleSequenceActor *);
void func_ov006_02070c58(void *);
void func_ov006_02070c70(void *);
void MTX_RotZ44_(void *, int, int);
void func_02036ca4(void *);
void func_ov006_02070bd4(s32 *, s32 *, int, int, int);
extern void *data_ov006_0207c4e4;
extern const TitleSpriteLayout data_ov006_0207b2e4;
extern const u8 data_ov006_0207aff8[];
void *func_ov006_0206b77c(void *, int, int, int, int, int, u32 *);
void func_02036cc0(const void *);
int func_ov006_02070ba4(int, int, int);

/* Preserve the native nullable deletion guard, as in battle lifecycle cleanup. */
static inline void DeleteSpriteAnimation(GameSpriteAnimation *animation)
{
    if (animation) {
        GameSpriteAnimation_Finalize(animation);
        GameHeap_Delete(animation);
    }
}

void TitleSequenceActor_Update(TitleSequenceActor *work)
{
    switch (work->header.state) {
    case 0:
        break;
    case 1:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            work->current_scale = sine;
            work->alpha = 20 * sine / 4096;
        } else {
            int x, y;
            work->current_scale = 4096;
            work->alpha = 20;
            x = work->header.x / 4096;
            y = work->header.y / 4096;
            work->tracks[1] =
                GameSpriteAnimation_Start(work->animation, work->commands[1], x + 128, y + 100, 2048, 256);
            work->tracks[0] =
                GameSpriteAnimation_Start(work->animation, work->commands[0], x + 128, y + 116, 2048, 256);
            work->tracks[1]->speed = _s32_div_f(0x1000000, work->timing_scale) / 16;
            work->tracks[0]->speed = _s32_div_f(0x1000000, work->timing_scale) / 16;
            work->header.elapsed = 0;
            work->header.duration = 60 * work->timing_scale / 4096;
            work->header.state = 2;
        }
        break;
    case 2:
        work->current_scale = (FX_SinCosTable_[2 * (work->angle >> 4)] << 9) / 4096 + 4096;
        work->alpha = 20;
        work->angle += 4096;
        ++work->header.elapsed;
        if (work->header.elapsed >= work->header.duration) {
            work->start_scale = work->current_scale;
            work->header.elapsed = 0;
            work->header.duration = 8 * work->timing_scale / 4096;
            work->header.state = 3;
        }
        break;
    case 3:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            work->current_scale = work->start_scale - sine * work->start_scale / 4096;
            work->alpha =
                20 *
                FX_SinCosTable_[2 *
                                ((_s32_div_f(work->header.elapsed << 14, work->header.duration) + 0x4000) >>
                                 4)] /
                4096;
        } else {
            work->header.elapsed = 0;
            work->header.duration = 0;
            work->header.state = 0;
        }
        break;
    }
}

void TitleSequenceActor_Draw(TitleSequenceActor *work)
{
    REG32(0x04000444) = 0;
    REG32(0x04000440) = 2;
    func_02036cc0(data_ov006_0207aff8);
    GameSpriteAnimation_Update(work->animation);
    REG32(0x04000448) = 1;
    if (!work->header.state)
        return;
    if (!work->current_scale)
        return;
    if (!work->alpha)
        return;
    REG32(0x04000440) = 3;
    REG32(0x04000454) = 0;
    REG32(0x04000440) = 2;
    REG32(0x04000444) = 0;
    REG32(0x04000454) = 0;
    REG32(0x04000480) = 0x7fff;
    REG32(0x04000444) = 0;
    Translate(work->header.x / 16 + 0x8000, work->header.y / 16 + 0x5400, 2304);
    Scale(work->current_scale, 3072 * work->current_scale / 4096, 4096);
    TitleTexture_DrawLayout(&work->texture, work->layout, work->alpha, 18);
    REG32(0x04000448) = 1;
}

void TitleSequenceActor_Init(TitleSequenceActor *work)
{
    GameSpriteAnimation *animation = GameHeap_New(sizeof(GameSpriteAnimation), 1, 0, 0);
    int bank;
    if (animation)
        animation = GameSpriteAnimation_Initialize(animation, work->track_storage, 4, 1024);
    work->animation = animation;
    work->commands[0] = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 15, 0, 0, 1, 0);
    work->commands[1] = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 16, 0, 0, 1, 0);
    TitleTexture_Load(&work->texture, 13, 14, 1);
    bank = func_02035818();
    TitleTexture_Upload(&work->texture);
    func_02035c00(bank);
    work->header.update = (void (*)(void *))TitleSequenceActor_Update;
    work->header.draw = (void (*)(void *))TitleSequenceActor_Draw;
    work->layout = &data_ov006_0207b2e4;
    work->start_scale = 0;
    work->current_scale = 0;
    work->angle = 0;
    work->alpha = 0;
    work->header.state = 0;
}

void TitleSequenceActor_Release(TitleSequenceActor *work)
{
    GameSpriteAnimation *animation = work->animation;
    if (animation) {
        DeleteSpriteAnimation(animation);
        work->animation = 0;
    }
    TitleTexture_Release(&work->texture);
    if (work->commands[0]) {
        GameHeap_Free((void *)work->commands[0]);
        work->commands[0] = 0;
    }
    if (work->commands[1]) {
        GameHeap_Free((void *)work->commands[1]);
        work->commands[1] = 0;
    }
}

void TitleSequenceActor_Start(TitleSequenceActor *work, int x, int y, int timing_scale)
{
    work->timing_scale = timing_scale;
    work->header.elapsed = 0;
    work->header.duration = 12 * work->timing_scale / 4096;
    work->header.x = x << 12;
    work->header.y = y << 12;
    work->header.state = 1;
}

void TitleSequenceActor_Stop(TitleSequenceActor *work)
{
    if (work->tracks[1])
        work->tracks[1]->commands = 0;
    if (work->tracks[0])
        work->tracks[0]->commands = 0;
    work->header.state = 0;
}

int TitleSequenceActor_IsHolding(TitleSequenceActor *work)
{
    return work->header.state == 2;
}

void TitleSequenceSprite_StartEntry(TitleSequenceSprite *work)
{
    work->header.x = work->start_x << 12;
    work->header.y = work->start_y << 12;
    work->header.elapsed = 0;
    work->header.duration = work->movement_frames;
    func_ov006_02070bd4(&work->acceleration, &work->velocity, work->unknown_3c, work->unknown_40,
                        work->header.duration);
    work->scale_y = 512;
    work->scale_x = work->scale_y;
    work->alpha = 0;
    work->header.state = 2;
}

void TitleSequenceSprite_QueueEntry(TitleSequenceSprite *work, int delay, int movement_frames,
                                    int hold_frames, int start_x, int start_y, int target_x, int target_y,
                                    int unknown_3c, int unknown_40)
{
    work->header.elapsed = 0;
    work->header.duration = delay;
    work->movement_frames = movement_frames;
    work->hold_frames = hold_frames;
    work->start_x = start_x;
    work->start_y = start_y;
    work->target_x = target_x;
    work->target_y = target_y;
    work->unknown_3c = unknown_3c;
    work->unknown_40 = unknown_40;
    work->header.state = 1;
}

void TitleSequenceSprite_StartMove(TitleSequenceSprite *work, int start_x, int start_y, int target_x,
                                   int target_y, int duration)
{
    work->start_x = start_x;
    work->start_y = start_y;
    work->target_x = target_x;
    work->target_y = target_y;
    work->header.elapsed = 0;
    work->header.duration = duration;
    work->header.state = 5;
}

int TitleSequenceSprite_StartSquash(TitleSequenceSprite *work, int duration)
{
    work->start_x = work->header.x / 4096;
    work->start_y = work->header.y / 4096;
    work->header.elapsed = 0;
    work->header.duration = duration;
    work->header.state = 6;
    return 1536 * work->layout->height / 4096;
}

void TitleSequenceSprite_Update(TitleSequenceSprite *work)
{
    switch (work->header.state) {
    case 0:
        break;
    case 1:
        ++work->header.elapsed;
        if (work->header.elapsed >= work->header.duration) {
            TitleSequenceSprite_StartEntry(work);
            work->header.state = 2;
        }
        break;
    case 2:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int start_x, sine;
            sine = FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            work->scale_y = 3584 * sine / 4096 + 512;
            work->scale_x = work->scale_y;
            work->vertical_offset =
                func_ov006_02070ba4(work->acceleration, work->velocity, work->header.elapsed);
            work->header.y = (work->start_y << 12) - work->vertical_offset;
            start_x = work->start_x;
            sine = FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            work->header.x = sine * (work->target_x - start_x) + (start_x << 12);
            work->alpha = 31;
            if (work->velocity / 2 + work->acceleration * work->header.elapsed < 0)
                work->header.depth = work->final_depth;
        } else {
            work->header.x = work->target_x << 12;
            work->header.y = work->target_y << 12;
            work->scale_y = 4096;
            work->scale_x = work->scale_y;
            work->alpha = 31;
            work->header.elapsed = 0;
            work->header.duration = work->hold_frames;
            work->header.state = 3;
        }
        break;
    case 3:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 15, work->header.duration) >> 4)];
            work->scale_x = (sine << 10) / 4096 + 4096;
            work->scale_y = (-sine << 10) / 4096 + 4096;
            work->header.y = (work->target_y << 12) + (4096 - work->scale_y) * work->layout->height / 2;
        } else {
            work->scale_y = 4096;
            work->scale_x = work->scale_y;
            work->header.y = work->target_y << 12;
            work->header.state = 4;
        }
        break;
    case 4:
        break;
    case 5:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            int squash;
            work->header.x = sine * (work->target_x - work->start_x) + (work->start_x << 12);
            work->header.y = sine * (work->target_y - work->start_y) + (work->start_y << 12);
            squash =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 15, work->header.duration) >> 4)];
            work->scale_x = -squash * 768 / 4096 + 4096;
            work->scale_y = 768 * squash / 4096 + 4096;
            work->header.y += (4096 - work->scale_y) * work->layout->height / 2;
        } else {
            work->header.x = work->target_x << 12;
            work->header.y = work->target_y << 12;
            work->scale_x = 4096;
            work->scale_y = 4096;
            work->header.state = 4;
        }
        break;
    case 6:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 15, work->header.duration) >> 4)];
            work->scale_x = 1536 * sine / 4096 + 4096;
            work->scale_y = -sine * 1536 / 4096 + 4096;
            work->header.y = (work->start_y << 12) + (4096 - work->scale_y) * work->layout->height / 2;
        } else {
            work->header.x = work->start_x << 12;
            work->header.y = work->start_y << 12;
            work->scale_x = 4096;
            work->scale_y = 4096;
            work->header.state = 4;
        }
        break;
    case 16:
        ++work->header.elapsed;
        if (work->header.elapsed >= work->header.duration) {
            work->alpha = 0;
            work->header.state = 0;
        } else {
            work->alpha =
                31 - 31 *
                         FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >>
                                              4)] /
                         4096;
        }
        break;
    }
}

void TitleSequenceSprite_Draw(TitleSequenceSprite *work)
{
    if (!work->header.state)
        return;
    if (!work->scale_x)
        return;
    if (!work->scale_y)
        return;
    if (!work->alpha)
        return;
    REG32(0x04000440) = 3;
    REG32(0x04000454) = 0;
    REG32(0x04000440) = 2;
    REG32(0x04000444) = 0;
    REG32(0x04000454) = 0;
    REG32(0x04000480) = 0x7fff;
    REG32(0x04000444) = 0;
    Translate(work->header.x / 16, work->header.y / 16, work->header.depth);
    Scale(work->scale_x, work->scale_y, 4096);
    TitleTexture_DrawLayout(work->texture, work->layout, work->alpha, 16);
    REG32(0x04000448) = 1;
}

void TitleSequenceSprite_Init(TitleSequenceSprite *work, TitleTextureResource *texture, int index, int depth,
                              int final_depth)
{
    work->texture = texture;
    work->layout = &data_ov006_0207b2a4[index];
    work->acceleration = 0;
    work->velocity = 0;
    work->vertical_offset = 0;
    work->scale_x = 0;
    work->scale_y = 0;
    work->alpha = 0;
    work->header.update = (void (*)(void *))TitleSequenceSprite_Update;
    work->header.draw = (void (*)(void *))TitleSequenceSprite_Draw;
    work->header.x = 0;
    work->header.y = 0;
    work->header.depth = depth;
    work->final_depth = final_depth;
    work->header.state = 0;
}

int TitleSequenceSprite_IsIdle(TitleSequenceSprite *work)
{
    return work->header.state == 4;
}

int TitleSequenceSprite_IsSquashing(TitleSequenceSprite *work)
{
    return work->header.state == 3;
}

void TitleRotatingSprite_StartArc(TitleRotatingSprite *work, int duration, int hold_frames, int start_x,
                                  int start_y, int target_x, int target_y, int unknown_3c, int unknown_40,
                                  int state)
{
    work->sprite.start_x = start_x;
    work->sprite.start_y = start_y;
    work->sprite.target_x = target_x;
    work->sprite.target_y = target_y;
    work->sprite.header.x = start_x << 12;
    work->sprite.header.y = start_y << 12;
    work->sprite.header.elapsed = 0;
    work->sprite.header.duration = duration;
    work->sprite.hold_frames = hold_frames;
    work->sprite.unknown_3c = unknown_3c;
    work->sprite.unknown_40 = unknown_40;
    func_ov006_02070bd4(&work->sprite.acceleration, &work->sprite.velocity, work->sprite.unknown_3c,
                        work->sprite.unknown_40, work->sprite.header.duration);
    work->sprite.header.state = state;
}

void TitleRotatingSprite_StartVerticalMove(TitleRotatingSprite *work, int distance)
{
    work->sprite.start_x = work->sprite.header.x / 4096;
    work->sprite.start_y = work->sprite.header.y / 4096;
    work->sprite.target_x = work->sprite.header.x / 4096;
    work->sprite.target_y = distance + work->sprite.header.y / 4096;
    work->sprite.header.elapsed = 0;
    work->sprite.header.duration = 10;
    work->sprite.header.state = 2;
}

void TitleRotatingSprite_StartSquash(TitleRotatingSprite *work, int duration)
{
    work->sprite.target_x = work->sprite.header.x / 4096;
    work->sprite.target_y = work->sprite.header.y / 4096;
    work->sprite.header.elapsed = 0;
    work->sprite.header.duration = duration;
    work->sprite.header.state = 4;
}

int TitleRotatingSprite_IsIdle(TitleRotatingSprite *work)
{
    return work->sprite.header.state == 8;
}

void TitleRotatingSprite_Update(TitleRotatingSprite *rotating)
{
    TitleSequenceSprite *work = &rotating->sprite;
    switch (work->header.state) {
    case 0:
        break;
    case 1:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int start_x, sine;
            work->vertical_offset =
                func_ov006_02070ba4(work->acceleration, work->velocity, work->header.elapsed);
            work->header.y = (work->start_y << 12) - work->vertical_offset;
            start_x = work->start_x;
            sine = FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            work->header.x = sine * (work->target_x - start_x) + (start_x << 12);
        } else {
            work->header.x = work->target_x << 12;
            work->header.y = work->target_y << 12;
            work->header.state = 8;
        }
        break;
    case 2:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            int squash;
            work->header.x = sine * (work->target_x - work->start_x) + (work->start_x << 12);
            work->header.y = sine * (work->target_y - work->start_y) + (work->start_y << 12);
            squash =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 15, work->header.duration) >> 4)];
            work->scale_x = 1536 * squash / 4096 + 4096;
            work->scale_y = -squash * 1536 / 4096 + 4096;
            work->header.y += (4096 - work->scale_y) * work->layout->height / 2;
        } else {
            work->header.x = work->target_x << 12;
            work->header.y = work->target_y << 12;
            work->scale_x = 4096;
            work->scale_y = 4096;
            TitleRotatingSprite_StartArc(rotating, 30, 16, work->header.x / 4096, work->header.y / 4096, 229,
                                         80, 48, -(80 - work->header.y / 4096), 3);
        }
        break;
    case 3:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int start_x, sine;
            work->vertical_offset =
                func_ov006_02070ba4(work->acceleration, work->velocity, work->header.elapsed);
            work->header.y = (work->start_y << 12) - work->vertical_offset;
            start_x = work->start_x;
            sine = FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            work->header.x = sine * (work->target_x - start_x) + (start_x << 12);
            rotating->angle = _s32_div_f(work->header.elapsed << 16, work->header.duration);
        } else {
            work->header.x = work->target_x << 12;
            work->header.y = work->target_y << 12;
            rotating->angle = 0;
            TitleRotatingSprite_StartSquash(rotating, work->hold_frames);
        }
        break;
    case 4:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 15, work->header.duration) >> 4)];
            work->scale_x = (sine << 10) / 4096 + 4096;
            work->scale_y = (-sine << 10) / 4096 + 4096;
            work->header.y = (work->target_y << 12) + (4096 - work->scale_y) * work->layout->height / 2;
        } else {
            work->scale_y = 4096;
            work->scale_x = work->scale_y;
            work->header.y = work->target_y << 12;
            work->header.state = 8;
        }
        break;
    case 8:
        break;
    case 16:
        ++work->header.elapsed;
        if (work->header.elapsed >= work->header.duration) {
            work->alpha = 0;
            work->header.state = 0;
        } else {
            work->alpha =
                31 - 31 *
                         FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >>
                                              4)] /
                         4096;
        }
        break;
    }
}

void TitleRotatingSprite_Draw(TitleRotatingSprite *work)
{
    s32 matrix[16];
    int scale_x, scale_y;
    if (!work->sprite.header.state)
        return;
    if (!work->sprite.alpha)
        return;
    REG32(0x04000440) = 3;
    REG32(0x04000454) = 0;
    REG32(0x04000440) = 2;
    REG32(0x04000444) = 0;
    REG32(0x04000454) = 0;
    REG32(0x04000480) = 0x7fff;
    REG32(0x04000444) = 0;
    Translate(work->sprite.header.x / 16, work->sprite.header.y / 16, 1552);
    scale_y = work->sprite.scale_y;
    scale_x = work->sprite.scale_x;
    REG32(0x0400046c) = scale_x;
    REG32(0x0400046c) = scale_y;
    REG32(0x0400046c) = 4096;
    MTX_RotZ44_(matrix, FX_SinCosTable_[2 * (work->angle >> 4)], FX_SinCosTable_[2 * (work->angle >> 4) + 1]);
    func_02036ca4(matrix);
    TitleTexture_DrawLayout(work->sprite.texture, work->sprite.layout, work->sprite.alpha, 16);
    REG32(0x04000448) = 1;
}

void TitleRotatingSprite_Init(TitleRotatingSprite *work, TitleTextureResource *texture)
{
    work->sprite.texture = texture;
    work->sprite.layout = data_ov006_0207b2d4;
    work->sprite.header.update = (void (*)(void *))TitleRotatingSprite_Update;
    work->sprite.header.draw = (void (*)(void *))TitleRotatingSprite_Draw;
    work->sprite.header.x = 0;
    work->sprite.header.y = 0;
    work->sprite.alpha = 31;
    work->sprite.scale_y = 4096;
    work->sprite.scale_x = work->sprite.scale_y;
    work->angle = 0;
    work->sprite.header.state = 0;
}

void TitleFadeSprite_Update(TitleFadeSprite *work)
{
    switch (work->header.state) {
    case 0:
        break;
    case 1:
        ++work->header.elapsed;
        if (work->header.elapsed >= work->header.duration) {
            work->alpha = 31;
            work->header.state = 2;
        } else {
            work->alpha =
                31 *
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)] /
                4096;
        }
        break;
    case 2:
        break;
    }
}

void TitleFadeSprite_Draw(TitleFadeSprite *work)
{
    if (!work->header.state)
        return;
    if (!work->alpha)
        return;
    REG32(0x04000440) = 3;
    REG32(0x04000454) = 0;
    REG32(0x04000440) = 2;
    REG32(0x04000444) = 0;
    REG32(0x04000454) = 0;
    REG32(0x04000480) = 0x7fff;
    REG32(0x04000444) = 0;
    Translate(work->header.x / 16, work->header.y / 16, 1536);
    TitleTexture_DrawLayout(work->texture, work->layout, work->alpha, 17);
    REG32(0x04000448) = 1;
}

void TitleFadeSprite_Init(TitleFadeSprite *work, TitleTextureResource *texture)
{
    work->texture = texture;
    work->layout = data_ov006_0207b2c4;
    work->header.update = (void (*)(void *))TitleFadeSprite_Update;
    work->header.draw = (void (*)(void *))TitleFadeSprite_Draw;
    work->header.x = 0x80000;
    work->header.y = 278528;
    work->header.state = 0;
}

void TitleFadeSprite_StartFadeIn(TitleFadeSprite *work)
{
    work->header.elapsed = 0;
    work->header.duration = 32;
    work->header.state = 1;
}

void TitleFadeSprite_Show(TitleFadeSprite *work)
{
    work->alpha = 31;
    work->header.state = 2;
}

void TitleSpriteSequence_Update(TitleSpriteSequence *work)
{
    ++work->elapsed;
    if (work->header.state > 1)
        ++work->active_elapsed;
    work->actor.header.update(&work->actor);
    work->sprites[0].header.update(&work->sprites[0]);
    work->sprites[1].header.update(&work->sprites[1]);
    work->rotating.sprite.header.update(&work->rotating);
    work->fade.header.update(&work->fade);
    switch (work->header.state) {
    case 0:
        break;
    case 1:
        if (work->elapsed > 435) {
            TitleSequenceActor_Start(&work->actor, 0, 0, 4096);
            work->header.state = 2;
        }
        break;
    case 2:
        if (TitleSequenceActor_IsHolding(&work->actor)) {
            TitleSequenceSprite_QueueEntry(&work->sprites[0], 16, 40, 20, 128, 84, 128, 76, 64, 8);
            TitleSequenceSprite_QueueEntry(&work->sprites[1], 40, 40, 16, 128, 84, 220, 72, 80, 12);
            work->header.state = 3;
        }
        break;
    case 3:
        if (TitleSequenceSprite_IsSquashing(&work->sprites[1])) {
            TitleSequenceSprite_StartMove(&work->sprites[0], 128, 76, 114, 76, 16);
            work->header.state = 4;
        }
        break;
    case 4:
        if (TitleSequenceSprite_IsIdle(&work->sprites[0]) && TitleSequenceSprite_IsIdle(&work->sprites[1]) &&
            func_ov006_02075120(&work->actor)) {
            TitleRotatingSprite_StartArc(&work->rotating, 24, 0, 219, -64, 219, 40, 0, -104, 1);
            work->header.state = 5;
        }
        break;
    case 5:
        if (TitleRotatingSprite_IsIdle(&work->rotating)) {
            int duration = TitleSequenceSprite_StartSquash(&work->sprites[1], 20);
            TitleRotatingSprite_StartVerticalMove(&work->rotating, duration);
            work->header.state = 7;
        }
        break;
    case 6:
        break;
    case 7:
        if (TitleSequenceSprite_IsIdle(&work->sprites[1])) {
            TitleSequenceSprite_StartMove(&work->sprites[0], 114, 76, 100, 76, 12);
            TitleSequenceSprite_StartMove(&work->sprites[1], 220, 72, 206, 72, 12);
            work->header.state = 8;
        }
        break;
    case 8:
        if (TitleSequenceSprite_IsIdle(&work->sprites[0]) && TitleSequenceSprite_IsIdle(&work->sprites[1]) &&
            TitleRotatingSprite_IsIdle(&work->rotating)) {
            TitleFadeSprite_StartFadeIn(&work->fade);
            work->header.state = 9;
        }
        break;
    case 9:
        if (work->fade.header.state == 2) {
            func_ov006_02070c58(&work->sprites[0]);
            func_ov006_02070c58(&work->sprites[1]);
            func_ov006_02070c58(&work->rotating);
            work->header.state = 0;
        }
        break;
    }
}

void TitleSpriteSequence_Draw(TitleSpriteSequence *work)
{
    work->actor.header.draw(&work->actor);
    work->sprites[0].header.draw(&work->sprites[0]);
    work->sprites[1].header.draw(&work->sprites[1]);
    work->rotating.sprite.header.draw(&work->rotating);
    work->fade.header.draw(&work->fade);
}

void TitleSpriteSequence_Init(TitleSpriteSequence *work)
{
    int bank;
    TitleTexture_Load(&work->textures[0], 21, 23, 1);
    TitleTexture_Load(&work->textures[1], 22, 23, 1);
    bank = func_02035818();
    TitleTexture_Upload(&work->textures[0]);
    TitleTexture_Upload(&work->textures[1]);
    func_02035c00(bank);
    TitleSequenceActor_Init(&work->actor);
    TitleSequenceSprite_Init(&work->sprites[0], &work->textures[0], 0, 1584, 1584);
    TitleSequenceSprite_Init(&work->sprites[1], &work->textures[0], 1, 1600, 1568);
    TitleRotatingSprite_Init(&work->rotating, &work->textures[1]);
    TitleFadeSprite_Init(&work->fade, &work->textures[0]);
    work->header.update = (void (*)(void *))TitleSpriteSequence_Update;
    work->header.draw = (void (*)(void *))TitleSpriteSequence_Draw;
    work->header.state = 0;
}

void TitleSpriteSequence_Release(TitleSpriteSequence *work)
{
    TitleTexture_Release(&work->textures[0]);
    TitleTexture_Release(&work->textures[1]);
    TitleSequenceActor_Release(&work->actor);
}

void TitleSpriteSequence_Start(TitleSpriteSequence *work)
{
    work->header.state = 1;
}

void TitleSpriteSequence_Finish(TitleSpriteSequence *work)
{
    TitleSequenceActor_Stop(&work->actor);
    func_ov006_02070c70(&work->sprites[0]);
    func_ov006_02070c70(&work->sprites[1]);
    func_ov006_02070c70(&work->rotating);
    TitleFadeSprite_Show(&work->fade);
    work->header.state = 0;
}

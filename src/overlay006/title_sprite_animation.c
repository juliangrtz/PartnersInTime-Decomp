#include "title_sprite_render_internal.h"
#include <game/heap.h>
extern void *data_ov006_0207c4e4;
extern const TitleSpriteLayout data_ov006_0207b2e4;
extern const u8 data_ov006_0207aff8[];
extern const s16 FX_SinCosTable_[];
int _s32_div_f(int, int);
void *func_ov006_0206b77c(void *, int, int, int, int, int, u32 *);
void func_ov006_020718fc(TitleTextureResource *);
int func_02035818(void);
void func_02035c00(int);
void func_ov006_02070bd4(s32 *, s32 *, int, int, int);
void func_ov006_0207193c(TitleTextureResource *, const void *, int, int);
void func_02036cc0(const void *);

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
    func_ov006_0207193c(&work->texture, work->layout, work->alpha, 18);
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
    func_ov006_020718fc(&work->texture);
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

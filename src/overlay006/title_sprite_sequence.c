#include "title_sprite_render_internal.h"
#include <game/heap.h>
extern const TitleSpriteLayout data_ov006_0207b2a4[], data_ov006_0207b2c4[], data_ov006_0207b2d4[];
extern const s16 FX_SinCosTable_[];
int _s32_div_f(int, int);
void func_ov006_020718fc(TitleTextureResource *);
int func_02035818(void);
void func_02035c00(int);
int func_ov006_02075120(TitleSequenceActor *);
void func_ov006_02070c58(void *);
void func_ov006_02070c70(void *);
void func_ov006_0206f710(TitleRotatingSprite *);
void func_ov006_0207193c(TitleTextureResource *, const void *, int, int);
void MTX_RotZ44_(void *, int, int);
void func_02036ca4(void *);

/* Preserve the native nullable deletion guard, as in battle lifecycle cleanup. */
static inline void DeleteSpriteAnimation(GameSpriteAnimation *animation)
{
    if (animation) {
        GameSpriteAnimation_Finalize(animation);
        GameHeap_Delete(animation);
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
    func_ov006_0207193c(work->sprite.texture, work->sprite.layout, work->sprite.alpha, 16);
    REG32(0x04000448) = 1;
}

void TitleRotatingSprite_Init(TitleRotatingSprite *work, TitleTextureResource *texture)
{
    work->sprite.texture = texture;
    work->sprite.layout = data_ov006_0207b2d4;
    work->sprite.header.update = (void (*)(void *))func_ov006_0206f710;
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
    func_ov006_0207193c(work->texture, work->layout, work->alpha, 17);
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
    func_ov006_020718fc(&work->textures[0]);
    func_ov006_020718fc(&work->textures[1]);
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

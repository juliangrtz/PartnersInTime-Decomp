#include <game/title_localized_sequence.h>
#include <game/heap.h>
#include "title_graphics_internal.h"
#define REG16(address) (*(volatile u16 *)(address))
extern u8 *data_ov006_0207c4e4;
void *func_ov006_0206b77c(void *, int, int, int, int, int, u32 *);
int func_02035818(void);
void func_02035c00(int);
extern const s16 data_ov006_0207b0f0[];
extern const u8 data_ov006_0207afb8[];
extern const s16 FX_SinCosTable_[];
int _s32_div_f(int, int);
void func_02036cc0(const void *);
void func_02036ca4(const void *);
void MTX_RotZ44_(void *, int, int);

static inline void DeleteSpriteAnimation(GameSpriteAnimation *animation)
{
    if (animation) {
        GameSpriteAnimation_Finalize(animation);
        GameHeap_Delete(animation);
    }
}

void TitleLocalizedSequence_Draw(TitleLocalizedSequence *work)
{
    int actor_alpha = 0, actor_scale = 0;
    int scale_x = 4096, scale_y = 4096, title_alpha = 31;
    int final_scale = 4096, final_alpha = 31;
    int subtitle_y = data_ov006_0207b0f0[data_ov006_0207c4e4[1048] - 1] << 8;
    s32 matrix[16];
    if (work->header.state)
        ++work->active_elapsed;
    REG32(0x04000444) = 0;
    REG32(0x04000440) = 2;
    func_02036cc0(data_ov006_0207afb8);
    GameSpriteAnimation_Update(work->animation);
    REG32(0x04000448) = 1;
    REG32(0x04000440) = 3;
    REG32(0x04000454) = 0;
    REG32(0x04000440) = 2;
    REG32(0x04000444) = 0;
    REG32(0x04000454) = 0;
    REG32(0x04000480) = 0x7fff;
    switch (work->header.state) {
    case 0:
    case 4:
        break;
    case 1:
        if (work->header.elapsed < 515)
            ++work->header.elapsed;
        else {
            work->header.elapsed = 0;
            work->header.state = 2;
        }
        break;
    case 2:
        work->header.elapsed = 0;
        work->header.duration = 12;
        work->header.state = 3;
        break;
    case 3:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            actor_scale =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            actor_alpha = 20 * actor_scale / 4096;
        } else {
            work->header.state = 5;
            actor_scale = 4096;
            actor_alpha = 20;
        }
        break;
    case 5:
        work->tracks[1] = GameSpriteAnimation_Start(work->animation, work->commands[1], 128, 88, 2048, 256);
        work->tracks[0] = GameSpriteAnimation_Start(work->animation, work->commands[0], 128, 104, 2048, 256);
        actor_scale = (FX_SinCosTable_[2 * (work->pulse_angle >> 4)] << 9) / 4096 + 4096;
        actor_alpha = 20;
        work->pulse_angle += 4096;
        work->header.elapsed = 0;
        work->header.duration = 20;
        work->header.state = 6;
        break;
    case 6:
        actor_scale = (FX_SinCosTable_[2 * (work->pulse_angle >> 4)] << 9) / 4096 + 4096;
        actor_alpha = 20;
        work->pulse_angle += 4096;
        if (work->header.elapsed < work->header.duration)
            ++work->header.elapsed;
        else {
            int speed;
            work->header.x = 128 << 12;
            work->header.y = 72 << 12;
            work->visible_parts.bits.part_0 = 1;
            work->header.elapsed = 0;
            work->header.duration = 40;
            REG16(0x040002b0) = 0;
            REG32(0x040002b8) = 3145728;
            while (REG16(0x040002b0) & 0x8000) {
            }
            speed = REG32(0x040002b4);
            speed += 2048;
            work->acceleration = ((256 - speed) << 13) / 1600;
            work->velocity = (speed << 13) / 40;
            scale_x = scale_y = title_alpha = 0;
            work->header.state = 7;
        }
        break;
    case 7:
        actor_scale = (FX_SinCosTable_[2 * (work->pulse_angle >> 4)] << 9) / 4096 + 4096;
        actor_alpha = 20;
        work->pulse_angle += 4096;
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int elapsed = work->header.elapsed;
            int angle = _s32_div_f(elapsed << 14, work->header.duration);
            scale_x = scale_y = FX_SinCosTable_[2 * (angle >> 4)];
            {
                int displacement = (work->velocity + work->acceleration * elapsed) / 2;
                displacement /= 16;
                displacement *= elapsed;
                work->vertical_offset = displacement / 4096;
            }
            work->header.y = (72 << 12) - (work->vertical_offset << 12);
        } else {
            work->header.y = 56 << 12;
            work->start_scale = actor_scale;
            work->header.elapsed = 0;
            work->header.duration = 20;
            scale_x = scale_y = 4096;
            work->header.state = 8;
        }
        break;
    case 8:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int elapsed = work->header.elapsed, duration = work->header.duration;
            int angle = _s32_div_f(elapsed << 14, duration);
            int sine = FX_SinCosTable_[2 * (angle >> 4)];
            /* Retain the native 32-bit temporary across the phase-to-cosine lookup. */
            int cosine = angle + 16384;
            cosine = FX_SinCosTable_[2 * (cosine >> 4)];
            actor_scale = work->start_scale - sine * work->start_scale / 4096;
            cosine *= 20;
            actor_alpha = cosine / 4096;
            sine = FX_SinCosTable_[2 * (_s32_div_f(elapsed << 15, duration) >> 4)];
            scale_x = (sine << 10) / 4096 + 4096;
            scale_y = (-sine << 10) / 4096 + 4096;
        } else {
            work->header.elapsed = 0;
            work->header.duration = 24;
            scale_x = scale_y = 4096;
            work->header.state = 9;
        }
        break;
    case 9:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int separation;
            int sine =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            int alpha;
            separation = 256 - sine / 16;
            alpha = (sine << 4) / 4096;
            REG32(0x04000444) = 0;
            Translate((128 + separation) << 8, subtitle_y, 1280);
            Scale(4096, sine, 4096);
            TitleTexture_DrawQuad(&work->textures[1], 2, 0, 252, 32, alpha, 17);
            REG32(0x04000448) = 1;
            REG32(0x04000444) = 0;
            Translate((128 - separation) << 8, subtitle_y, 1280);
            Scale(4096, sine, 4096);
            TitleTexture_DrawQuad(&work->textures[1], 2, 0, 252, 32, alpha, 18);
            REG32(0x04000448) = 1;
        } else {
            work->visible_parts.bits.part_1 = 1;
            work->visible_parts.bits.part_3 = 1;
            work->visible_parts.bits.part_2 = work->visible_parts.bits.part_3;
            final_scale = 0;
            final_alpha = 0;
            work->header.elapsed = 0;
            work->header.duration = 16;
            work->header.state = 10;
        }
        break;
    case 10:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            final_scale =
                FX_SinCosTable_[2 * (_s32_div_f(work->header.elapsed << 14, work->header.duration) >> 4)];
            final_alpha = 31 * final_scale / 4096;
        } else {
            work->header.elapsed = 0;
            work->header.duration = 0;
            work->header.state = 0;
        }
        break;
    }
    if (actor_scale > 0) {
        int height = 3072 * actor_scale / 4096;
        REG32(0x04000444) = 0;
        Translate(128 << 8, 72 << 8, 2304);
        Scale(actor_scale, height, 4096);
        TitleTexture_DrawQuad(&work->textures[3], 0, 0, 128, 64, actor_alpha, 21);
        REG32(0x04000448) = 1;
    }
    if (work->visible_parts.bits.part_2) {
        int height = 3072 * final_scale / 4096;
        REG32(0x04000444) = 0;
        Translate(128 << 8, 72 << 8, 1792);
        Scale(final_scale, height, 4096);
        MTX_RotZ44_(matrix, FX_SinCosTable_[2 * (work->rotation >> 4)],
                    FX_SinCosTable_[2 * (work->rotation >> 4) + 1]);
        func_02036ca4(matrix);
        work->rotation += 170;
        TitleTexture_DrawQuad(&work->textures[2], 0, 0, 128, 128, 16, 19);
        REG32(0x04000448) = 1;
    }
    if (work->visible_parts.bits.part_3) {
        REG32(0x04000444) = 0;
        Translate(246 << 8, 40 << 8, 1280);
        TitleTexture_DrawQuad(&work->textures[4], 0, 0, 16, 8, final_alpha, 20);
        REG32(0x04000448) = 1;
    }
    if (work->visible_parts.bits.part_0) {
        REG32(0x04000444) = 0;
        Translate(work->header.x / 16, work->header.y / 16, 1536);
        Scale(scale_x, scale_y, 4096);
        TitleTexture_DrawQuad(&work->textures[0], 8, 0, 240, 64, title_alpha, 16);
        REG32(0x04000448) = 1;
    }
    if (work->visible_parts.bits.part_1) {
        REG32(0x04000444) = 0;
        Translate(128 << 8, subtitle_y, 1280);
        TitleTexture_DrawQuad(&work->textures[1], 2, 0, 252, 32, 31, -1);
        REG32(0x04000448) = 1;
    }
    REG32(0x04000448) = 1;
}

void TitleLocalizedSequence_Init(TitleLocalizedSequence *work)
{
    GameSpriteAnimation *animation = GameHeap_New(sizeof(GameSpriteAnimation), 1, 0, 0);
    int bank;
    if (animation)
        animation = GameSpriteAnimation_Initialize(animation, work->track_storage, 4, 1024);
    work->animation = animation;
    TitleTexture_Load(&work->textures[0], 17, 20, 1);
    TitleTexture_Load(&work->textures[2], 19, 20, 1);
    TitleTexture_Load(&work->textures[3], 13, 14, 1);
    TitleTexture_Load(&work->textures[4], 30, 20, 1);
    TitleTexture_Load(&work->textures[1], data_ov006_0207c4e4[1048] + 24, 20, 1);
    bank = func_02035818();
    TitleTexture_Upload(&work->textures[0]);
    TitleTexture_Upload(&work->textures[2]);
    TitleTexture_Upload(&work->textures[3]);
    TitleTexture_Upload(&work->textures[4]);
    TitleTexture_Upload(&work->textures[1]);
    func_02035c00(bank);
    work->commands[0] = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 15, 0, 0, 1, 0);
    work->commands[1] = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 16, 0, 0, 1, 0);
    work->header.x = 0x80000;
    work->header.y = 0x38000;
    work->header.draw = (void (*)(void *))TitleLocalizedSequence_Draw;
}

void TitleLocalizedSequence_Release(TitleLocalizedSequence *work)
{
    GameSpriteAnimation *animation = work->animation;
    if (animation) {
        DeleteSpriteAnimation(animation);
        work->animation = 0;
    }
    TitleTexture_Release(&work->textures[0]);
    TitleTexture_Release(&work->textures[2]);
    TitleTexture_Release(&work->textures[3]);
    TitleTexture_Release(&work->textures[4]);
    TitleTexture_Release(&work->textures[1]);
    if (work->commands[0]) {
        GameHeap_Free((void *)work->commands[0]);
        work->commands[0] = 0;
    }
    if (work->commands[1]) {
        GameHeap_Free((void *)work->commands[1]);
        work->commands[1] = 0;
    }
}

void TitleLocalizedSequence_Start(TitleLocalizedSequence *work)
{
    work->header.state = 1;
}

void TitleLocalizedSequence_Finish(TitleLocalizedSequence *work)
{
    work->visible_parts.mask = 0xff;
    work->header.x = 0x80000;
    work->header.y = 0x38000;
    if (work->tracks[0])
        work->tracks[0]->commands = 0;
    if (work->tracks[1])
        work->tracks[1]->commands = 0;
    work->header.state = 0;
}

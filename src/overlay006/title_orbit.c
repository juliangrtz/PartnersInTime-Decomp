#include <game/title_model.h>
#include <game/title_effects.h>
#include "title_trail_internal.h"

extern const s16 FX_SinCosTable_[];
extern const s16 data_ov006_0207b0dc[], data_ov006_0207b0d4[];
extern const u8 data_ov006_0207b0b0[];
extern const s16 data_ov006_0207af3c[2][7];
void func_ov006_0207214c(TitleOrbitWork *);
void func_ov006_02073e6c(TitleOrbitWork *, int, int);

void TitleOrbit_Draw(TitleOrbitWork *work)
{
    work->depth = 1024;
    work->main_model->rotation_z = work->rotation;
    work->sub_model->rotation_z = work->rotation;
    switch (work->render_phase) {
    case 0:
        func_ov006_02073e6c(work, 0, 1);
        TitleModel_Draw(work);
        break;
    case 1:
        func_ov006_02073e6c(work, 0, 1);
        TitleModel_Draw(work);
        if (--work->countdown <= 0) {
            work->child->x = work->orbit_x;
            work->child->y = work->orbit_y;
            TitleTrailStamp_Draw(work->child);
            work->countdown = 2;
        }
        break;
    case 2:
    case 3: {
        int x, y;
        func_ov006_02073e6c(work, 0, 1);
        TitleModel_Draw(work);
        x = work->x;
        y = work->y;
        work->x = work->orbit_x;
        work->y = work->orbit_y;
        work->depth = 1040;
        func_ov006_02073e6c(work, data_ov006_0207b0b0[work->flags.side], 1);
        TitleModel_Draw(work);
        work->x = x;
        work->y = y;
        work->depth = 0;
        break;
    }
    }
}

void TitleOrbit_Init(TitleOrbitWork *work, TitleOrbitChild *child, int side, int slot)
{
    TitleModel_Load(work, 0xc2000033, 3, 0, 1);
    work->child = child;
    work->x = data_ov006_0207b0d4[side] << 12;
    work->y = -262144;
    work->orbit_x = work->x;
    work->orbit_y = work->y;
    work->fall_velocity = 0;
    work->start_angle = 0;
    if (side)
        work->angle = 81920 - (slot << 15) / 6;
    else
        work->angle = (slot << 15) / 6 - 49152;
    work->render_phase = 0;
    work->flags.side = (u8)side;
    work->flags.slot = (u8)slot;
    work->update = func_ov006_0207214c;
    work->draw = TitleOrbit_Draw;
}

void TitleOrbit_StartEntry(TitleOrbitWork *work)
{
    work->timer = data_ov006_0207af3c[work->flags.side][work->flags.slot];
    work->phase = 1;
}
void TitleOrbit_StartExpansion(TitleOrbitWork *work)
{
    work->timer = 0;
    work->render_phase = 2;
    work->phase = 6;
}
void TitleOrbit_StartPulse(TitleOrbitWork *work)
{
    work->timer = 0;
    work->phase = 7;
}
void TitleOrbit_Reset(TitleOrbitWork *work)
{
    BattleModel *model;
    work->orbit_x = 92 * FX_SinCosTable_[2 * ((work->angle & 0xffff) >> 4) + 1] +
                    (data_ov006_0207b0dc[work->flags.side] << 12);
    work->orbit_y = 92 * FX_SinCosTable_[2 * ((work->angle & 0xffff) >> 4)] + 0x100000;
    work->x = 124 * FX_SinCosTable_[2 * ((work->angle & 0xffff) >> 4) + 1] +
              (data_ov006_0207b0dc[work->flags.side] << 12);
    work->y = 124 * FX_SinCosTable_[2 * ((work->angle & 0xffff) >> 4)] + 0x100000;
    work->rotation = 0;
    work->timer = 0;
    work->render_phase = 3;
    model = work->main_model;
    model->scale_x = 256;
    model->scale_y = 256;
    model = work->sub_model;
    model->scale_x = 256;
    model->scale_y = 256;
    work->phase = 0;
}
void TitleOrbit_Destroy(TitleOrbitWork *work)
{
    TitleModel_Release(work);
}

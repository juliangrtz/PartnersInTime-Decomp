#include <game/title_model.h>
#include <game/title_panel_motion.h>

extern void func_ov006_0206e9e4(TitleMovingSprite *);
extern void TitleMovingSprite_Draw(TitleMovingSprite *);
extern void TitleMovingSprite_ResetOrbit(TitleMovingSprite *);
extern const s8 data_ov006_0207b0b8[];
extern const s16 FX_SinCosTable_[], data_ov006_0207b0c8[];
extern const u8 data_ov006_0207b0a0[], data_ov006_0207b0a4[], data_ov006_0207b0a8[];
void func_ov006_02073e6c(void *, int, int);
extern const s16 data_ov006_0207b0cc[];

void TitleMovingSprite_Draw(TitleMovingSprite *work)
{
    int side = work->flags.side;
    if (!work->flags.variant) {
        work->depth = 768;
        func_ov006_02073e6c(work, data_ov006_0207b0a4[side], 1);
        TitleModel_Draw(work);
    }
    switch (work->state) {
    case 0:
    case 1:
        break;
    case 2: {
        int rotation = (work->angle - 0x8000) & 65535;
        work->main_model->rotation_z = rotation;
        work->sub_model->rotation_z = rotation;
        if (work->flags.variant) {
            work->depth = 832;
            func_ov006_02073e6c(work, data_ov006_0207b0a8[side], 1);
            TitleModel_Draw(work);
        } else {
            int x = work->x;
            int y = work->y;
            work->x = x + (-FX_SinCosTable_[2 * (rotation >> 4) + 1] << 6);
            work->y += -FX_SinCosTable_[2 * (rotation >> 4)] << 6;
            work->depth = 800;
            func_ov006_02073e6c(work, data_ov006_0207b0a0[side], 1);
            TitleModel_Draw(work);
            work->x = x;
            work->y = y;
        }
        work->main_model->rotation_z = 0;
        work->sub_model->rotation_z = 0;
        ((TitleMovingSpriteChild *)work->child)->x =
            data_ov006_0207b0c8[work->flags.variant] * FX_SinCosTable_[2 * (work->angle >> 4) + 1] + work->x;
        ((TitleMovingSpriteChild *)work->child)->y =
            data_ov006_0207b0c8[work->flags.variant] * FX_SinCosTable_[2 * (work->angle >> 4)] + work->y;
        ((TitleMovingSpriteChild *)work->child)->y += 0x10000;
        ((TitleMovingSpriteChild *)work->child)->depth = work->flags.variant ? 816 : 784;
        TitleModel_Draw(work->child);
        break;
    }
    }
}

void TitleMovingSprite_Init(TitleMovingSprite *work, void *child, int side, int variant, void *parent)
{
    int origin;
    TitleModel_Load(work, 0xc2000033, 3, 0, 1);
    work->update = func_ov006_0206e9e4;
    work->draw = TitleMovingSprite_Draw;
    work->flags.side = (u8)side;
    work->flags.variant = (u8)variant;
    work->child = child;
    work->sub_model->flags &= ~(1u << 26);
    work->parent = parent;
    TitleMovingSprite_ResetOrbit(work);
    origin = data_ov006_0207b0cc[work->flags.side];
    origin += data_ov006_0207b0b8[work->flags.side] << 7;
    work->x = origin << 12;
    work->frame = 355;
}

void TitleMovingSprite_StartEntry(TitleMovingSprite *work)
{
    int origin = data_ov006_0207b0cc[work->flags.side];
    origin += data_ov006_0207b0b8[work->flags.side] << 7;
    work->x = origin << 12;
    work->elapsed = 0;
    work->state = 1;
}

void TitleMovingSprite_StartOrbit(TitleMovingSprite *work)
{
    TitleMovingSprite_ResetOrbit(work);
    work->frame = -1;
    work->state = 2;
}

void TitleMovingSprite_Destroy(TitleMovingSprite *work)
{
    TitleModel_Release(work);
}

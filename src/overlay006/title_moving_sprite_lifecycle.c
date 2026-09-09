#include <game/title_panel_motion.h>

extern void func_ov006_02074280(TitleMovingSprite *, u32, u8, int, int);
extern void func_ov006_0206e9e4(TitleMovingSprite *);
extern void func_ov006_0206e7b8(TitleMovingSprite *);
extern void func_ov006_0206ec20(TitleMovingSprite *);
extern void func_ov006_020741a4(TitleMovingSprite *);
extern const s8 data_ov006_0207b0b8[];
extern const s16 data_ov006_0207b0cc[];

void TitleMovingSprite_Init(TitleMovingSprite *work, void *child, int side, int variant, void *parent)
{
    int origin;
    func_ov006_02074280(work, 0xc2000033, 3, 0, 1);
    work->update = func_ov006_0206e9e4;
    work->draw = func_ov006_0206e7b8;
    work->flags.side = (u8)side;
    work->flags.variant = (u8)variant;
    work->child = child;
    work->sub_model->flags &= ~(1u << 26);
    work->parent = parent;
    func_ov006_0206ec20(work);
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
    func_ov006_0206ec20(work);
    work->frame = -1;
    work->state = 2;
}

void TitleMovingSprite_Destroy(TitleMovingSprite *work)
{
    func_ov006_020741a4(work);
}

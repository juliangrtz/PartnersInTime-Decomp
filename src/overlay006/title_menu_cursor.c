#include "title_menu_internal.h"
#include <game/audio.h>
extern const s16 FX_SinCosTable_[];
void func_ov006_02073e6c(void *, int, int);
static inline void SetScale(BattleModel *model, s16 scale)
{
    model->scale_x = scale;
    model->scale_y = scale;
}
void TitleMenuCursor_Update(void *element)
{
    TitleMenuCursor *work = element;
    switch (work->header.state) {
    case TITLE_CURSOR_IDLE:
        break;
    case TITLE_CURSOR_MOVING:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int start_x = work->start_x;
            int sine = FX_SinCosTable_[2 * (((work->header.elapsed << 14) / work->header.duration) >> 4)];
            work->header.x = start_x + sine * ((work->target_x - start_x) / 4096);
            work->header.y = work->start_y + sine * ((work->target_y - work->start_y) / 4096);
        } else {
            work->header.x = work->target_x;
            work->header.y = work->target_y;
            work->header.state = TITLE_CURSOR_IDLE;
        }
        break;
    case TITLE_CURSOR_APPEARING:
        ++work->header.elapsed;
        if (work->header.elapsed < work->header.duration) {
            int sine = FX_SinCosTable_[2 * (((work->header.elapsed << 14) / work->header.duration) >> 4)];
            work->header.x = work->start_x + sine * ((work->target_x - work->start_x) / 4096);
            work->header.y = work->start_y + sine * ((work->target_y - work->start_y) / 4096);
            SetScale(work->sub_model, sine / 16);
        } else {
            SetScale(work->sub_model, 256);
            work->header.x = work->target_x;
            work->header.y = work->target_y;
            work->header.state = TITLE_CURSOR_IDLE;
        }
        break;
    }
}
void TitleMenuCursor_Draw(void *element)
{
    TitleModel_Draw(element);
}
void TitleMenuCursor_Move(TitleMenuCursor *work, int x, int y, int frames)
{
    work->start_x = work->header.x;
    work->start_y = work->header.y;
    work->target_x = x;
    work->target_y = y;
    work->header.elapsed = 0;
    work->header.duration = frames;
    GameAudio_PlayEffectDelayed(231, 0, -1);
    work->header.state = TITLE_CURSOR_MOVING;
}
void TitleMenuCursor_Show(TitleMenuCursor *work, int x, int y, int frames)
{
    work->target_x = x;
    work->target_y = y;
    work->start_x = x - (16 << 12);
    work->start_y = y;
    SetScale(work->sub_model, 0);
    work->header.depth = 256;
    work->header.elapsed = 0;
    work->header.duration = frames;
    work->header.pass_flags = 0;
    work->header.state = TITLE_CURSOR_APPEARING;
}
void TitleMenuCursor_Init(TitleMenuCursor *work)
{
    TitleModel_Load(work, 0xc2000033, 2, 0, 1);
    func_ov006_02073e6c(work, 13, 1);
    work->header.pass_flags = 255;
    work->header.update = TitleMenuCursor_Update;
    work->header.draw = TitleMenuCursor_Draw;
}
void TitleMenuCursor_Release(TitleMenuCursor *work)
{
    TitleModel_Release(work);
}

#include <game/title_model.h>
#include <game/sprite_output.h>
#include <game/battle_effect.h>
extern "C" {
#include <game/heap.h>
extern const s16 FX_SinCosTable_[];
extern u8 data_02060740[];
void MTX_RotZ44_(MtxFx44 *, int, int);
void MTX_Identity44_(MtxFx44 *);
void func_020093b4(BattleModel *, int);
BattleModel *func_ov006_0206ae44(void *, u32, int, int, int);
extern void *data_ov006_0207c4e4;
extern BattleModelVTable data_ov006_0207b3c0;
}

struct TitleModelElement {
    void *next;
    s32 elapsed, duration, x, y, depth;
    void (*update)(void *);
    void (*draw)(void *);
    void (*upload)(void *);
    u8 state, draw_flags, screen_mask, unknown_27;
    BattleModel *sub_model, *main_model;
};
/* The sub-screen model stores a sprite palette at the shared model offset 0x130. */
struct TitleModelPaletteView {
    u8 unknown_00[304];
    GameSpritePalette palette;
};
typedef char TitleModelElement_Size[sizeof(TitleModelElement) == 48 ? 1 : -1];
typedef char TitleModelPaletteView_Size[sizeof(TitleModelPaletteView) == 324 ? 1 : -1];
/* Keep the original separate null guard around the virtual deleting call. */
static inline void DeleteModel(BattleModel *model)
{
    if (model)
        model->unknown_14();
}
static inline void SetModelPosition(BattleModel *model, int x, int y)
{
    model->animation_offset_x = x;
    model->animation_offset_y = y;
}
extern "C" {
BattleModel *TitleModelController_Delete(BattleModel *);

void TitleModel_Load(void *element, u32 resource, u8 screens, int parameter, int mode)
{
    TitleModelElement *work = (TitleModelElement *)element;
    work->screen_mask = screens;
    if (screens & 1) {
        work->main_model = func_ov006_0206ae44(data_ov006_0207c4e4, resource, 0, parameter, mode);
        func_020093b4(work->main_model, 1);
    }
    if (screens & 2) {
        work->sub_model = func_ov006_0206ae44(data_ov006_0207c4e4, resource, 1, parameter, mode);
        func_020093b4(work->sub_model, 1);
    }
    work->draw = TitleModel_Draw;
}

BattleModel *TitleModelController_Delete(BattleModel *model)
{
    *(const void **)model = &data_ov006_0207b3c0;
    BattleModelController_DestroyBase(model);
    GameHeap_Delete(model);
    return model;
}

void TitleModel_Release(void *element)
{
    TitleModelElement *work = (TitleModelElement *)element;
    if (work->main_model) {
        work->main_model->stop();
        if (work->main_model) {
            DeleteModel(work->main_model);
            work->main_model = 0;
        }
    }
    if (work->sub_model) {
        work->sub_model->stop();
        TitleModelPaletteView *view = (TitleModelPaletteView *)work->sub_model;
        if (view->palette.count)
            GameSpritePalette_Unlink(&view->palette);
        if (work->sub_model) {
            DeleteModel(work->sub_model);
            work->sub_model = 0;
        }
    }
}

void TitleModel_DrawMain(BattleModel *model)
{
    MtxFx44 rotation;
    MtxFx44 *matrix = (MtxFx44 *)model->get_sprite_transform();
    if (model->flag_bits.facing_left) {
        matrix->_00 = -matrix->_00;
        matrix->_10 = -matrix->_10;
        matrix->_20 = -matrix->_20;
        matrix->_30 = -matrix->_30;
    }
    if (model->flag_bits.flip_y) {
        matrix->_01 = -matrix->_01;
        matrix->_11 = -matrix->_11;
        matrix->_21 = -matrix->_21;
        matrix->_31 = -matrix->_31;
    }
    MTX_TransApply44(matrix, matrix, model->animation_offset_x << 8, model->animation_offset_y << 8,
                     model->render_anchor_z);
    MTX_RotZ44_(&rotation, FX_SinCosTable_[2 * (model->rotation_z >> 4)],
                FX_SinCosTable_[2 * (model->rotation_z >> 4) + 1]);
    MTX_Concat44(&rotation, matrix, matrix);
    MTX_ScaleApply44(matrix, matrix, 16 * model->scale_x, 16 * model->scale_y, 4096);
    model->draw(0, 0, 0);
    MTX_Identity44_(matrix);
}

void TitleModel_DrawSub(BattleModel *model)
{
    u8 objects = data_0205a05c[1];
    u8 affines = data_0205a060[1];
    int original_objects = objects;
    int original_affines = affines;
    model->draw(data_02060740, &objects, &affines);
    GameOam_AddGroup(1, model->get_sort_key(), objects - original_objects, affines - original_affines);
}

void TitleModel_Draw(void *element)
{
    TitleModelElement *work = (TitleModelElement *)element;
    if (work->main_model) {
        SetModelPosition(work->main_model, work->x / 4096, work->y / 4096);
        work->main_model->render_anchor_z = work->depth;
        TitleModel_DrawMain(work->main_model);
    }
    if (work->sub_model) {
        SetModelPosition(work->sub_model, work->x / 4096, work->y / 4096 - 244);
        work->sub_model->render_anchor_z = work->depth;
        TitleModel_DrawSub(work->sub_model);
    }
}


}

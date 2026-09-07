#include <game/battle_scene.h>
extern "C" {
#include <game/heap.h>
#include <game/sprite_output.h>
}

/* The head of a model, as the constructors see it before the virtual table has
   been installed. */
struct BattleModelStorage {
    BattleModelVTable *vtable;
    BattleModel *render_previous;
    BattleModel *render_next;
    BattleSceneObject *owner;
    u8 unknown_010;
};

extern "C" {

extern BattleModelVTable data_02050970;

void func_02009148(BattleModelStorage *model)
{
    model->vtable = &data_02050970;
    model->unknown_010 = 0xff;
    model->render_next = 0;
    model->render_previous = model->render_next;
    model->owner = 0;
}

void func_02009138(BattleModelStorage *model)
{
    model->vtable = &data_02050970;
}

void func_020090f8(int screen)
{
    BattleModel *model = gModelRenderList[screen];
    if (model == 0) return;
    do {
        model->prepare_render();
        model = model->render_next;
    } while (model != 0);
}

void func_020090b8(int screen)
{
    BattleModel *model = gModelRenderList[screen];
    if (model == 0) return;
    do {
        model->unknown_04();
        model = model->render_next;
    } while (model != 0);
}

void func_02009094(int screen)
{
    gModelRenderListTail[screen] = 0;
    gModelRenderList[screen] = gModelRenderListTail[screen];
}

void func_02009078(int screen, u8 *extended, u8 *standard)
{
    data_0205a08c[screen] = extended;
    data_0205a094[screen] = standard;
}

void func_02009058(int screen)
{
    data_0205a08c[screen] = 0;
    data_0205a094[screen] = 0;
}

void DisplayVram_ClearEngineAssignments(int screen)
{
    data_0205a06c[screen] = 0;
    data_0205a074[screen] = 0;
    data_0205a07c[screen] = 0;
    data_0205a084[screen] = 0;
    data_0205a08c[screen] = 0;
    data_0205a094[screen] = 0;
}

void func_02008fc4(void)
{
    data_0205a068 = (GameOamEntry *)GameHeap_Allocate(2, 0x400, 0, 0);
}

}

#include <game/title_panel_motion.h>
#include <game/random.h>

extern const s16 FX_SinCosTable_[];
extern TitlePanelLayoutValue data_ov006_0207af58[], data_ov006_0207af5a[], data_ov006_0207af5c[];
void func_ov006_0206e09c(TitlePanel *);
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
int _s32_div_f(int, int);

void TitlePanel_ResetPulse(TitlePanel *panel)
{
    panel->alpha = 16;
    panel->elapsed = 0;
    panel->duration = Random_NextModulo(624) + 16;
    panel->state = 1;
}

void TitlePanel_RecordTrail(TitlePanel *panel)
{
    void *source = &panel->trail[0];
    void *destination = &panel->trail[1];
    if ((u32)source < (u32)destination)
        func_0202cd2c(source, destination, 40);
    else
        func_0202cc58(source, destination, 40);
    panel->trail[0].x = panel->x / 4096;
    panel->trail[0].y = panel->y / 4096;
    panel->trail[0].active = 1;
}

int TitlePanel_DrainTrail(TitlePanel *panel)
{
    void *source, *destination;
    if (!panel->trail[1].active)
        return 1;
    source = &panel->trail[1];
    destination = &panel->trail[0];
    if ((u32)source < (u32)destination)
        func_0202cd2c(source, destination, 40);
    else
        func_0202cc58(source, destination, 40);
    panel->trail[5].active = 0;
    return 0;
}

void TitlePanel_Update(TitlePanel *panel)
{
    switch (panel->state) {
    case 1:
        if (++panel->elapsed >= panel->duration) {
            panel->elapsed = 0;
            panel->duration = Random_NextModulo(8) + 4;
            panel->state = (u32)Random_NextModulo(10) < 2 ? 3 : 2;
        }
        break;
    case 2:
        if (++panel->elapsed < panel->duration)
            panel->alpha =
                15 * FX_SinCosTable_[2 * (_s32_div_f(panel->elapsed << 15, panel->duration) >> 4)] / 4096 +
                16;
        else
            TitlePanel_ResetPulse(panel);
        break;
    case 3:
        if (++panel->elapsed < panel->duration)
            panel->alpha =
                -12 * FX_SinCosTable_[2 * (_s32_div_f(panel->elapsed << 15, panel->duration) >> 4)] / 4096 +
                16;
        else
            TitlePanel_ResetPulse(panel);
        break;
    case 4:
        if (++panel->elapsed < panel->duration)
            panel->alpha =
                16 * FX_SinCosTable_[2 * (_s32_div_f(panel->elapsed << 14, panel->duration) >> 4)] / 4096;
        else
            TitlePanel_ResetPulse(panel);
        break;
    case 5:
        if (++panel->elapsed < 20) {
            int factor = (panel->elapsed << 14) / 20;
            int start_x = panel->start_x;
            int delta_x = panel->target_x - start_x;
            factor = FX_SinCosTable_[2 * (factor >> 4)];
            panel->x = start_x + factor * delta_x / 4096;
            panel->y = panel->start_y + (panel->target_y - panel->start_y) * factor / 4096;
            TitlePanel_RecordTrail(panel);
        } else {
            panel->alpha = 0;
            if (TitlePanel_DrainTrail(panel)) {
                panel->elapsed = 0;
                panel->alpha = 0;
                panel->state = 0;
            }
        }
        break;
    }
}

void TitlePanel_Init(TitlePanel *panel, TitlePanelResources *resources, int index)
{
    panel->update = TitlePanel_Update;
    panel->draw = func_ov006_0206e09c;
    panel->x = data_ov006_0207af58[index].value << 12;
    panel->y = data_ov006_0207af5a[index].value << 12;
    panel->variant = data_ov006_0207af5c[index].value;
    panel->texture = resources->texture.offset;
    panel->palette = resources->palette.offset;
    panel->alpha = 0;
    panel->index = index;
    panel->parent = resources;
}

void func_0202cbd4(void *, int, u32);
extern TitleElementList *data_ov006_0207c4e0;

void TitlePanel_Release(TitlePanel *panel) {}

void TitlePanel_StartFadeIn(TitlePanel *panel)
{
    panel->elapsed = 0;
    panel->duration = 16;
    panel->state = 4;
}

void TitlePanel_StartPulse(TitlePanel *panel)
{
    TitlePanel_ResetPulse(panel);
}

void TitlePanel_TryStartTrail(TitlePanel *panel, int chance_denominator)
{
    int direction;
    if (panel->state)
        return;
    if (Random_NextModulo(chance_denominator))
        return;
    func_0202cbd4(panel->trail, 0, 48);
    panel->start_x = Random_NextModulo(192) + 32;
    panel->start_y = Random_NextModulo(32) - 16;
    direction = (u32)Random_NextModulo(100) < 50 ? -1 : 1;
    panel->target_x = Random_NextModulo(32) + 32;
    panel->target_x = panel->start_x + panel->target_x * direction;
    panel->target_y = Random_NextModulo(32) + 32;
    panel->start_x <<= 12;
    panel->start_y <<= 12;
    panel->target_x <<= 12;
    panel->target_y <<= 12;
    panel->variant = Random_NextModulo(2);
    panel->alpha = 24;
    panel->elapsed = 0;
    panel->state = 5;
}

void TitleElement_Append(TitleElementLink *panel)
{
    if (data_ov006_0207c4e0->first) {
        data_ov006_0207c4e0->last->next = panel;
        data_ov006_0207c4e0->last = panel;
        panel->next = 0;
    } else {
        data_ov006_0207c4e0->first = panel;
        data_ov006_0207c4e0->last = panel;
    }
}

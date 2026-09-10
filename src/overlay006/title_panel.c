#include <game/title_panel_motion.h>
#include <game/random.h>
#include <game/heap.h>
#include "title_graphics_internal.h"

extern const s16 FX_SinCosTable_[];
extern TitlePanelLayoutValue data_ov006_0207af58[], data_ov006_0207af5a[], data_ov006_0207af5c[];
void func_0202cc58(const void *, void *, u32);
void func_0202cd2c(const void *, void *, u32);
int _s32_div_f(int, int);
void func_0202cbd4(void *, int, u32);
extern TitleElementList *data_ov006_0207c4e0;
extern const u16 data_ov006_0207af38[];
extern void *data_ov006_0207c4e4;
extern u8 *data_ov006_0207b134;
void *func_ov006_0206b77c(void *, int, int, int, int, int, u32 *);
u32 func_02035818(void);
void func_02035c00(u32);

/* The original submits a quad using one XYZ vertex followed by XY vertices. */
static inline void DrawPanelQuad(int origin)
{
    REG32(0x04000500) = 1;
    TexCoord(origin, 0);
    Vertex(-8, -8, 0);
    TexCoord(origin, 0x10000);
    VertexXY(-8, 8);
    TexCoord(origin + 0x10000, 0x10000);
    VertexXY(8, 8);
    TexCoord(origin + 0x10000, 0);
    VertexXY(8, -8);
    REG32(0x04000504) = 0;
    REG32(0x04000448) = 1;
}

void TitlePanelResources_Load(TitlePanelResources *resources)
{
    void *pixels, *source, *destination;
    u32 bank, size;
    int i;
    resources->pixels = func_ov006_0206b77c(data_ov006_0207c4e4, 2, 11, 0, 0, 0, &resources->pixel_bytes);
    resources->pixel_bytes -= 16;
    pixels = GameHeap_Allocate(1, resources->pixel_bytes, 0, 0);
    source = (u8 *)resources->pixels + 16;
    size = resources->pixel_bytes;
    if ((u32)source < (u32)pixels)
        func_0202cd2c(source, pixels, size);
    else
        func_0202cc58(source, pixels, size);
    if (resources->pixels) {
        GameHeap_Free(resources->pixels);
        resources->pixels = 0;
    }
    resources->pixels = pixels;
    resources->palette_data =
        func_ov006_0206b77c(data_ov006_0207c4e4, 2, 12, 0, 0, 1, &resources->palette_bytes);
    GameTextureAllocation_Allocate(&resources->texture, 1, resources->pixel_bytes, 1, 467058, 0, 1, 0);
    GameTexturePalette_Allocate(&resources->palette, 0, 6, 8, 1, 1, 0, resources->palette_data, 467058, 0);
    bank = func_02035818();
    size = resources->pixel_bytes;
    destination = data_ov006_0207b134 + resources->texture.offset;
    source = resources->pixels;
    if ((u32)source < (u32)destination)
        func_0202cd2c(source, destination, size);
    else
        func_0202cc58(source, destination, size);
    func_02035c00(bank);
    for (i = 0; i < 6; ++i)
        resources->trail_alpha[i] = 24 + (-24 * i) / 6;
}

void TitlePanelResources_Release(TitlePanelResources *resources)
{
    if (resources->pixels) {
        GameHeap_Free(resources->pixels);
        resources->pixels = 0;
    }
    if (resources->palette_data) {
        GameHeap_Free(resources->palette_data);
        resources->palette_data = 0;
    }
    GameTextureAllocation_Unlink(&resources->texture);
    GameTexturePalette_Unlink(&resources->palette);
}

void TitlePanel_Draw(TitlePanel *panel)
{
    int texture_origin = data_ov006_0207af38[panel->variant] << 12;
    REG32(0x04000440) = 3;
    REG32(0x04000454) = 0;
    REG32(0x040004a8) = (panel->texture >> 3) | 0x58a00000;
    REG32(0x040004ac) = panel->palette >> 4;
    REG32(0x04000440) = 2;
    REG32(0x04000444) = 0;
    REG32(0x04000454) = 0;
    Scale(0x100000, 0x100000, 4096);
    if (panel->alpha) {
        REG32(0x04000444) = 0;
        Translate(panel->x / 4096, panel->y / 4096, 2560);
        PolygonAttributes(0, 0, 3, 2, panel->alpha, 0);
        DrawPanelQuad(texture_origin);
    }
    if (panel->state == 5) {
        int alpha_index = 5;
        int i = 5;
        for (; i >= 1; --i) {
            if (panel->trail[i].active) {
                int alpha;
                REG32(0x04000444) = 0;
                Translate(panel->trail[i].x, panel->trail[i].y, 2560 + i);
                alpha = ((TitlePanelResources *)panel->parent)->trail_alpha[alpha_index--];
                PolygonAttributes(0, 0, 3, i + 24, alpha, 0);
                DrawPanelQuad(texture_origin);
            }
        }
    }
    REG32(0x04000448) = 1;
}

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
    panel->draw = TitlePanel_Draw;
    panel->x = data_ov006_0207af58[index].value << 12;
    panel->y = data_ov006_0207af5a[index].value << 12;
    panel->variant = data_ov006_0207af5c[index].value;
    panel->texture = resources->texture.offset;
    panel->palette = resources->palette.offset;
    panel->alpha = 0;
    panel->index = index;
    panel->parent = resources;
}

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

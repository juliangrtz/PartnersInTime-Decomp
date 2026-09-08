#include "save_menu_internal.h"

SaveMenuText *SaveMenuText_Init(SaveMenuText *context)
{
    int i;
    func_ov005_02067468(*(void **)(data_ov005_0206a180 + 44), 0, data_ov008_020780bc[0], 1);
    if (!gSaveData[1301]) {
        context->owned_fonts[0] = func_ov005_020670b4(*(void **)(data_ov005_0206a180 + 44), 0, 3, 0, 0);
        context->owned_fonts[1] = func_ov005_020670b4(*(void **)(data_ov005_0206a180 + 44), 0, 4, 0, 0);
    } else {
        context->owned_fonts[0] = func_ov005_020670b4(*(void **)(data_ov005_0206a180 + 44), 0, 8, 0, 0);
        context->owned_fonts[1] = func_ov005_020670b4(*(void **)(data_ov005_0206a180 + 44), 0, 4, 0, 0);
    }
    func_ov005_02067424(*(void **)(data_ov005_0206a180 + 44), 0);
    context->fonts[0] = context->owned_fonts[0];
    context->fonts[1] = context->owned_fonts[1];
    context->fonts[12] = context->owned_fonts[0];
    context->fonts[13] = context->owned_fonts[1];
    for (i = 0; i < 3; ++i)
        context->strings[i] = func_ov005_0206687c(data_ov008_020780bc[i + 1], 0, 0);
    GameText_Init(&context->text, context->fonts, context->pixels, 0, 0, 0, 1, 4, 1, 6, 31, 0, 255, 0);
    return context;
}

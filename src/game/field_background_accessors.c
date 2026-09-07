#include <game/field_background.h>

void FieldBackground_SetScrollParameters(FieldBackground *background, s16 x, s16 y)
{
    background->scroll_parameter_x = x;
    background->scroll_parameter_y = y;
}

void FieldBackground_GetOrigin(FieldBackground *background, s16 *x, s16 *y)
{
    *x = background->origin_x >> 8;
    *y = background->origin_y >> 8;
}

int FieldBackground_IsReady(FieldBackground *background)
{
    return background->load_status == 101;
}

const FieldBackgroundConfiguration *FieldBackground_GetConfiguration(FieldBackground *background)
{
    return background->configuration;
}

const void *FieldBackground_GetResource650(FieldBackground *background)
{
    return !background->resource_650.size ? 0 : background->resource_650.data;
}

const void *FieldBackground_GetResource638(FieldBackground *background)
{
    return !background->resource_638.size ? 0 : background->resource_638.data;
}

int func_020133f8(FieldBackground *background)
{
    return 0;
}

#ifndef NITRO_GX_COMMAND_LIST_H
#define NITRO_GX_COMMAND_LIST_H

#include <nitro.h>

typedef struct GxCommandList {
    u8 *command;
    u32 *parameters;
    u8 *buffer;
    u32 unknown0c;
    u32 padding_required;
} GxCommandList;

#endif

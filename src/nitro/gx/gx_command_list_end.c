#include <nitro/gx_command_list.h>

u32 func_02038a04(GxCommandList *list) {
    if (list->buffer == list->command) return 0;
    switch ((u32)list->command & 3) {
    case 0:
        return list->command - list->buffer;
    case 1:
        *list->command++ = 0;
        /* fall through */
    case 2:
        *list->command++ = 0;
        /* fall through */
    case 3:
        *list->command++ = 0;
    }
    if (list->padding_required) {
        *list->parameters++ = 0;
        list->padding_required = 0;
    }
    list->command = (u8 *)list->parameters;
    return list->command - list->buffer;
}

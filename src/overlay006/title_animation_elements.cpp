#include "title_animation_internal.h"
#include <game/title_panel_motion.h>

struct TitlePassElement {
    TitlePassElement *next;
    u8 unknown_04[20];
    void (*update)(void *), (*draw)(void *), (*upload)(void *);
    u8 state;
    u8 unknown_0 : 1, draw_disabled : 1, upload_disabled : 1, unknown_3_7 : 5;
    u8 unknown_26[2];
};
typedef char TitlePassElement_Size[sizeof(TitlePassElement) == 40 ? 1 : -1];

extern "C" {
extern TitleElementList *data_ov006_0207c4e0;
extern TitleAnimationController *data_ov006_0207c4e4;

void TitleAnimation_DrawElements(void)
{
    TitlePassElement *element = (TitlePassElement *)data_ov006_0207c4e0->first;
    if (!element)
        return;
    do {
        if (element->draw && !element->draw_disabled)
            element->draw(element);
        element = element->next;
    } while (element);
}
void TitleAnimation_UploadElements(void)
{
    TitlePassElement *element = (TitlePassElement *)data_ov006_0207c4e0->first;
    if (!element)
        return;
    do {
        if (element->upload && !element->upload_disabled)
            element->upload(element);
        element = element->next;
    } while (element);
}
void TitleAnimation_RequestExit(int kind)
{
    data_ov006_0207c4e4->exit_state = 1;
    data_ov006_0207c4e4->exit_kind = kind;
}
}

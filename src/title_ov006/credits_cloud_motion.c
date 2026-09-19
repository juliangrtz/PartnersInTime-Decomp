#include "credits_transition_internal.h"

/* Keep each cloud within its variant's horizontal wrap interval. */
void CreditsCloud_Update(MenuElement *element)
{
    int *x = &element->arguments[1];
    element->arguments[1] += element->arguments[3];
    if (element->arguments[0]) {
        if (element->arguments[1] < -40 * 4096)
            *x += 296 * 4096;
    } else {
        if (element->arguments[1] < -88 * 4096)
            *x += 344 * 4096;
    }
}

/*
 * Credits cloud scrolling (overlay 6, 0x02079B98-0x02079BF8).
 *
 * Advances the clouds drifting across the credits backdrop.
 */

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

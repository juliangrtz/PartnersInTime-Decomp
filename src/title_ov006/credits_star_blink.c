#include "credits_transition_internal.h"
#include <game/random.h>

/* The bright star frame lasts four updates before a randomized pause. */
void CreditsStar_Update(MenuElement *element)
{
    switch (element->state) {
    case 0:
        if (element->counter) --element->counter;
        else {
            element->arguments[0] = 1;
            element->counter = 3;
            ++element->state;
        }
        break;
    case 1:
        if (element->counter) --element->counter;
        else {
            element->arguments[0] = 0;
            element->counter = Random_NextModulo(60) + (8 * Random_NextModulo(60) + 180);
            element->counter /= 2;
            element->state = 0;
        }
        break;
    }
}

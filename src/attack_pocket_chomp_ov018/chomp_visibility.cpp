#include "pocket_chomp_internal.h"

extern "C" {
void PocketChomp_UpdateVisibility(PocketChomp *center) {
    int phase = center->bits.phase;
    switch (phase) {
    case 1:
    case 2:
        center->object->render_state = 31;
        break;
    }
}
}

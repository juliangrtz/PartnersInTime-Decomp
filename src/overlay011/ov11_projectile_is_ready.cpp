#include "shell_internal.h"

extern "C" {
int Overlay11Projectile_IsReady(Overlay11Projectile *projectile, int wait_for_support)
{
    int state = projectile->state;
    switch (state) {
    case 3:
    case 4:
        if (wait_for_support) {
            if (!projectile->support->active)
                return 1;
        } else
            return 1;
        break;
    }
    return 0;
}
}

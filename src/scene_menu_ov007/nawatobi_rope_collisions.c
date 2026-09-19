#include "nawatobi_rope_internal.h"

void NawatobiRope_CheckLinkCollisions(NawatobiRopeLink *links, int count)
{
    int i;
    for (i = 0; i < count; ++i, ++links)
        func_ov007_0208aa78(links->first, links->second);
}

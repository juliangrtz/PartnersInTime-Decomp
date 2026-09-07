#include <nitro/ctrdg.h>

void CTRDG_Init(void) {
    if (!data_02065730) {
        data_02065730 = 1;
        CTRDGi_InitLock();
        func_0203d3ac();
        while (!func_0203d590(13, 1)) {}
        func_0203d5b8(13, CTRDGi_InitCallback);
        CTRDGi_InitModuleInfo();
        func_0203d5b8(13, 0);
        func_0203d5b8(13, CTRDGi_PulledOutCallback);
        data_02065734 = 0;
    }
}

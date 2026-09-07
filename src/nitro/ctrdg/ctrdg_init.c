#include <nitro/ctrdg.h>

void CTRDG_Init(void) {
    if (!data_02065730) {
        data_02065730 = 1;
        CTRDGi_InitLock();
        PXI_Init();
        while (!PXI_IsCallbackReady(13, 1)) {}
        PXI_SetFifoRecvCallback(13, CTRDGi_InitCallback);
        CTRDGi_InitModuleInfo();
        PXI_SetFifoRecvCallback(13, 0);
        PXI_SetFifoRecvCallback(13, CTRDGi_PulledOutCallback);
        data_02065734 = 0;
    }
}

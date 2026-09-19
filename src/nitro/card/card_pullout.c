/*
 * Card pullout handling (ARM9 resident, 0x02043A70-0x02043BB8).
 *
 * Detects the cartridge being removed and terminates cleanly instead of reading
 * from a socket that is no longer there.
 */

#include <nitro/card.h>

extern int data_02065720;
extern int (*data_02065724)(void);
extern void PXI_Init(void);
extern void WaitByLoop(u32 cycles);
extern int PM_ForceToPowerOff(void);

void CARDi_PulledOutCallback(u32 tag, u32 data, int error);
void CARD_TerminateForPulledOut(void);
void CARDi_SendPxi(u32 data, u32 delay);

void CARDi_InitPulledOutCallback(void) {
    PXI_Init();
    PXI_SetFifoRecvCallback(14, CARDi_PulledOutCallback);
    data_02065724 = 0;
}

void CARDi_PulledOutCallback(u32 tag, u32 data, int error) {
    if ((data & 63) == 17) {
        int terminate = 1;
        int (*callback)(void) = data_02065724;
        data_02065720 = 1;
        if (callback) terminate = callback();
        if (terminate) CARD_TerminateForPulledOut();
    } else OS_Terminate();
}

void CARD_TerminateForPulledOut(void) {
    if ((*(vu16 *)0x027FFFA8 & 0x8000) >> 15) PM_ForceToPowerOff();
    CARDi_SendPxi(1, 1);
    OS_Terminate();
}

void CARDi_SendPxi(u32 data, u32 delay) {
    while (PXI_SendWordByFifo(14, data, 0)) WaitByLoop(delay);
}

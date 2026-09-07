#include <nitro/ctrdg.h>

#define CTRDG_EXMEM (*(vu16 *)0x04000204)
#define CTRDG_MODULE (*(volatile CtrdgModuleInfo *)0x027FFC30)

typedef struct CtrdgGbaHeader {
    u8 reserved00[0xAC];
    u32 game_code;
    u16 maker;
    u8 marker;
    u8 reservedb3[11];
    u16 module_id;
} CtrdgGbaHeader;

static inline void CTRDGi_SetRomCycle(u32 cycle) { CTRDG_EXMEM = (cycle << 2) | (CTRDG_EXMEM & ~12); }
static inline void CTRDGi_SetSramCycle(u32 cycle) { CTRDG_EXMEM = (cycle << 4) | (CTRDG_EXMEM & ~16); }

void CTRDGi_InitCallback(u32 tag, u32 data, int error) {
    if ((data & 63) == 1) data_02065728.ready = 1;
    else OS_Terminate();
}

void CTRDGi_PulledOutCallback(u32 tag, u32 data, int error) {
    if ((data & 63) == 17) {
        int terminate = 0;
        if (data_02065734) terminate = data_02065734(0);
        if (terminate) CTRDG_TerminateForPulledOut();
    } else OS_Terminate();
}

void CTRDG_SetPulledOutCallback(CtrdgCallback callback) { data_02065734 = callback; }
void CTRDG_TerminateForPulledOut(void) { CTRDGi_SendPxi(2); OS_Terminate(); }

void CTRDGi_InitLock(void) {
    u32 zero = 0;
    CpuSet(&zero, &data_02065728, 0x05000001);
    data_02065728.lock_id = OS_GetLockID();
}

int CTRDG_IsOptionCartridge(u32 mask) { return CTRDG_IsExisting() && CTRDGi_TestModule(mask); }

int CTRDGi_TestModule(u32 mask) {
    int found = 0;
    u16 id = CTRDG_MODULE.module_id;
    if (id != 0xFFFF && id != 0 && (~*(vu8 *)0x027FFC30 & mask)) found = 1;
    return found;
}

int CTRDG_IsExisting(void) {
    int present = 1;
    volatile CtrdgModuleInfo *module = (volatile CtrdgModuleInfo *)0x027FFC30;
    const CtrdgGbaHeader *header;
    CtrdgLockState lock;
    CtrdgBusState bus;
    u8 marker;
    if (module->module_id == 0xFFFF) return 0;
    if (module->removed == 1) return 0;
    CTRDGi_Lock(data_02065728.lock_id, &lock);
    CTRDGi_SaveBus(&bus);
    header = (const CtrdgGbaHeader *)0x08000000;
    marker = header->marker;
    if ((marker == 0x96 && module->module_id != header->module_id) ||
        (marker != 0x96 && module->module_id != *(vu16 *)0x0801FFFE) ||
        (module->game_code != header->game_code && module->is_gba)) {
        present = 0;
        module->removed = 1;
    }
    CTRDGi_RestoreBus(&bus);
    CTRDGi_Unlock(data_02065728.lock_id, &lock);
    return present;
}

void CTRDGi_SaveBus(CtrdgBusState *state) {
    state->rom_cycle = (CTRDG_EXMEM & 12) >> 2;
    state->sram_cycle = (CTRDG_EXMEM & 16) >> 4;
    CTRDG_EXMEM = (CTRDG_EXMEM & ~12) | 12;
    CTRDG_EXMEM = CTRDG_EXMEM & ~16;
}

void CTRDGi_RestoreBus(const CtrdgBusState *state) {
    CTRDGi_SetRomCycle(state->rom_cycle);
    CTRDGi_SetSramCycle(state->sram_cycle);
}

void CTRDGi_Lock(u16 lock_id, CtrdgLockState *state) {
    for (;;) {
        state->interrupts = OS_DisableInterrupts();
        state->already_locked = OS_ReadOwnerOfLockWord((void *)0x027FFFE8) & 64;
        if (state->already_locked) return;
        if (!OS_TryLockCartridge(lock_id)) return;
        OS_RestoreInterrupts(state->interrupts);
        WaitByLoop(1);
    }
}

void CTRDGi_Unlock(u16 lock_id, CtrdgLockState *state) {
    if (!state->already_locked) func_02038e34(lock_id);
    OS_RestoreInterrupts(state->interrupts);
}

void CTRDGi_SendPxi(u32 data) {
    while (PXI_SendWordByFifo(13, data, 0)) WaitByLoop(1);
}

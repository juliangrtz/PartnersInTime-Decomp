#include <nitro/os_sync.h>

typedef void (*PxiCallback)(u32 tag, u32 data, int error);
typedef struct PxiSystemWork {
    u8 unknown00[0x388];
    vu32 callback_mask[2];
} PxiSystemWork;

extern u16 data_02064b84;
extern PxiCallback data_02064b88[32];
extern u32 OS_ResetRequestIrqMask(u32 mask);
extern void OS_SetIrqFunction(u32 mask, void (*callback)(void));
extern u32 OS_EnableIrqMask(u32 mask);
void func_0203d3b8(void);
void func_0203d624(void);

static inline PxiSystemWork *PxiGetSystemWork(void) {
    return (PxiSystemWork *)0x027FFC00;
}
#define PXI_SYSTEM PxiGetSystemWork()
#define PXI_SYNC (*(vu16 *)0x04000180)
#define PXI_CONTROL (*(vu16 *)0x04000184)

void func_0203d624(void) {
    u32 state = OS_DisableInterrupts();
    if (!data_02064b84) {
        int index;
        int timeout;
        u32 value;
        int stable;
        data_02064b84 = 1;
        PXI_SYSTEM->callback_mask[0] = 0;
        for (index = 0; index < 32; index++) data_02064b88[index] = 0;
        PXI_CONTROL = 0xC408;
        OS_ResetRequestIrqMask(0x40000);
        OS_SetIrqFunction(0x40000, func_0203d3b8);
        OS_EnableIrqMask(0x40000);
        for (stable = 0; ; stable++) {
            value = PXI_SYNC & 15;
            PXI_SYNC = value << 8;
            if (!value && stable > 4) break;
            timeout = 1000;
            while ((PXI_SYNC & 15) == value) {
                if (timeout <= 0) {
                    stable = 0;
                    break;
                }
                timeout--;
            }
        }
    }
    OS_RestoreInterrupts(state);
}

void func_0203d5b8(u32 tag, PxiCallback callback) {
    u32 state = OS_DisableInterrupts();
    data_02064b88[tag] = callback;
    if (callback) PXI_SYSTEM->callback_mask[0] |= 1 << tag;
    else PXI_SYSTEM->callback_mask[0] &= ~(1 << tag);
    OS_RestoreInterrupts(state);
}


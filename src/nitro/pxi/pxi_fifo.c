#include <nitro/os_sync.h>

typedef void (*PxiCallback)(u32 tag, u32 data, int error);
typedef union PxiFifoWord {
    u32 raw;
    struct { u32 tag : 5; u32 error : 1; u32 data : 26; } fields;
} PxiFifoWord;
extern PxiCallback data_02064b88[32];
void func_0203d3b8(void);
void func_0203d624(void);

#define PXI_CONTROL (*(vu16 *)0x04000184)
#define PXI_SEND (*(vu32 *)0x04000188)
#define PXI_RECEIVE (*(vu32 *)0x04100000)

static inline int SendFifoWord(PxiFifoWord *word) {
    u32 state;
    if (PXI_CONTROL & 0x4000) {
        PXI_CONTROL |= 0xC000;
        return -1;
    }
    state = OS_DisableInterrupts();
    if (PXI_CONTROL & 2) {
        OS_RestoreInterrupts(state);
        return -2;
    }
    PXI_SEND = word->raw;
    OS_RestoreInterrupts(state);
    return 0;
}

int func_0203d4dc(u32 tag, u32 data, int error) {
    PxiFifoWord word;
    word.fields.tag = tag;
    word.fields.error = error;
    word.fields.data = data;
    return SendFifoWord(&word);
}

static inline int ReceiveFifoWord(PxiFifoWord *word) {
    u32 state;
    if (PXI_CONTROL & 0x4000) {
        PXI_CONTROL |= 0xC000;
        return -3;
    }
    state = OS_DisableInterrupts();
    if (PXI_CONTROL & 0x100) {
        OS_RestoreInterrupts(state);
        return -4;
    }
    word->raw = PXI_RECEIVE;
    OS_RestoreInterrupts(state);
    return 0;
}

void func_0203d3b8(void) {
    PxiFifoWord word;
    for (;;) {
        int result = ReceiveFifoWord(&word);
        if (result == -4) return;
        if (result == -3) continue;
        if (word.fields.tag) {
            PxiCallback callback = data_02064b88[word.fields.tag];
            if (callback) callback(word.fields.tag, word.fields.data, word.fields.error);
            else if (!word.fields.error) {
                word.fields.error = 1;
                SendFifoWord(&word);
            }
        }
    }
}

void func_0203d3ac(void) { func_0203d624(); }

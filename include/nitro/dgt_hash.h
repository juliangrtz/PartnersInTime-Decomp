#ifndef NITRO_DGT_HASH_H
#define NITRO_DGT_HASH_H

#include <nitro.h>

typedef struct DgtHash2Context {
    u32 state[5];
    volatile u32 length_low;
    volatile u32 length_high;
    int block_index;
    u8 block[64];
    int computed;
    int error;
} DgtHash2Context;

typedef char DgtHash2ContextSizeCheck[(sizeof(DgtHash2Context) == 104) ? 1 : -1];

int DGT_Hash2Reset(DgtHash2Context *context);
int DGT_Hash2Input(DgtHash2Context *context, const u8 *data, u32 length);
int DGT_Hash2Result(DgtHash2Context *context, u8 *digest);
void DGTi_Hash2ProcessBlock(DgtHash2Context *context);
void DGTi_Hash2PadMessage(DgtHash2Context *context);
void DGT_Hash2CalcHmac(void *output, const void *data, u32 size, const void *key, int key_size);

#endif

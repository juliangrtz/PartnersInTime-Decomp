#include <nitro/dgt_hash.h>

typedef struct DgtHash2Constants { u32 words[4]; } DgtHash2Constants;
extern DgtHash2Constants data_02059da4;
extern int data_02064c84;

int DGT_Hash2Reset(DgtHash2Context *context) {
    if (!context) return 1;
    context->length_low = 0;
    context->length_high = 0;
    context->block_index = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->state[4] = 0xC3D2E1F0;
    context->computed = 0;
    context->error = 0;
    return 0;
}

int DGT_Hash2Result(DgtHash2Context *context, u8 *digest) {
    int i;
    if (!context || !digest) return 1;
    if (context->error) return context->error;
    if (!context->computed) {
        DGTi_Hash2PadMessage(context);
        for (i = 0; i < 64; i++) context->block[i] = 0;
        context->length_low = 0;
        context->length_high = 0;
        context->computed = 1;
    }
    for (i = 0; i < 20; i++) digest[i] = context->state[i >> 2] >> (8 * (3 - (i & 3)));
    return 0;
}

int DGT_Hash2Input(DgtHash2Context *context, const u8 *data, u32 length) {
    if (!length) return 0;
    if (!context || !data) return 1;
    if (context->computed) {
        context->error = 3;
        return 3;
    }
    if (context->error) return context->error;
    while (length-- && !context->error) {
        context->block[context->block_index++] = *data;
        context->length_low += 8;
        if (context->length_low == 0) {
            context->length_high++;
            if (context->length_high == 0) context->error = 1;
        }
        if (context->block_index == 64) DGTi_Hash2ProcessBlock(context);
        data++;
    }
    return 0;
}

void DGTi_Hash2ProcessBlock(DgtHash2Context *context) {
    u32 saved24;
    u32 saved56;
    DgtHash2Constants constants = data_02059da4;
    u32 words[80];
    u32 a, b, c, d, e;
    int i;
    u32 *block_words = (u32 *)context->block;
    if (data_02064c84) {
        saved24 = block_words[6];
        saved56 = block_words[14];
        block_words[6] = 0;
        block_words[14] = 0;
    }
    for (i = 0; i < 16; i++) {
        ((volatile u32 *)words)[i] = (u32)context->block[4 * i] << 24;
        ((volatile u32 *)words)[i] |= (u32)context->block[4 * i + 1] << 16;
        ((volatile u32 *)words)[i] |= (u32)context->block[4 * i + 2] << 8;
        ((volatile u32 *)words)[i] |= context->block[4 * i + 3];
    }
    for (i = 16; i < 80; i++) {
        u32 value = words[i - 16] ^ (words[i - 14] ^ (words[i - 3] ^ words[i - 8]));
        words[i] = (value << 1) | (value >> 31);
    }
    a = context->state[0];
    b = context->state[1];
    c = context->state[2];
    d = context->state[3];
    e = context->state[4];
    for (i = 0; i < 20; i++) {
        u32 value = constants.words[0] + (words[i] + (e + (((a << 5) | (a >> 27)) + ((b & c) | (~b & d)))));
        e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = value;
    }
    for (i = 20; i < 40; i++) {
        u32 value = constants.words[1] + (words[i] + (e + (((a << 5) | (a >> 27)) + (d ^ (b ^ c)))));
        e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = value;
    }
    for (i = 40; i < 60; i++) {
        u32 value = constants.words[2] + (words[i] + (e + (((a << 5) | (a >> 27)) + ((c & d) | (b & (c | d))))));
        e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = value;
    }
    for (i = 60; i < 80; i++) {
        u32 value = constants.words[3] + (words[i] + (e + (((a << 5) | (a >> 27)) + (d ^ (b ^ c)))));
        e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = value;
    }
    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
    context->state[4] += e;
    context->block_index = 0;
    if (data_02064c84) {
        block_words[6] = saved24;
        block_words[14] = saved56;
    }
}

void DGTi_Hash2PadMessage(DgtHash2Context *context) {
    if (context->block_index > 55) {
        context->block[context->block_index++] = 0x80;
        while (context->block_index < 64) context->block[context->block_index++] = 0;
        DGTi_Hash2ProcessBlock(context);
        while (context->block_index < 56) context->block[context->block_index++] = 0;
    } else {
        context->block[context->block_index++] = 0x80;
        while (context->block_index < 56) context->block[context->block_index++] = 0;
    }
    context->block[56] = context->length_high >> 24;
    context->block[57] = context->length_high >> 16;
    context->block[58] = context->length_high >> 8;
    context->block[59] = context->length_high;
    context->block[60] = context->length_low >> 24;
    context->block[61] = context->length_low >> 16;
    context->block[62] = context->length_low >> 8;
    context->block[63] = context->length_low;
    DGTi_Hash2ProcessBlock(context);
}

void DGT_Hash2CalcHmac(void *output, const void *data, u32 size, const void *key, int key_size) {
    u8 inner[64];
    u8 outer[64];
    u8 digest_storage[20];
    DgtHash2Context context;
    const u8 *key_bytes = (const u8 *)key;
    u8 *digest = digest_storage;
    int i;
    if (!output || !data || !size || !key || !key_size) return;
    if (key_size > 64) {
        DGT_Hash2Reset(&context);
        DGT_Hash2Input(&context, key_bytes, key_size);
        DGT_Hash2Result(&context, digest);
        key_bytes = digest;
        key_size = 20;
    }
    for (i = 0; i < key_size; i++) {
        inner[i] = *key_bytes ^ 0x36;
        outer[i] = *key_bytes++ ^ 0x5C;
    }
    for (; i < 64; i++) {
        inner[i] = 0x36;
        outer[i] = 0x5C;
    }
    DGT_Hash2Reset(&context);
    DGT_Hash2Input(&context, inner, 64);
    DGT_Hash2Input(&context, (const u8 *)data, size);
    DGT_Hash2Result(&context, digest);
    DGT_Hash2Reset(&context);
    DGT_Hash2Input(&context, outer, 64);
    DGT_Hash2Input(&context, digest, 20);
    DGT_Hash2Result(&context, (u8 *)output);
}

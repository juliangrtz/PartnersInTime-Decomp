#include <game/audio.h>

extern void MIi_CpuCopy16(const void *source, void *destination, u32 size);
extern s16 data_02049f2c;
extern const u16 data_02049f80[];
extern s16 data_0205ac94;
extern u16 data_0205ac96[];
extern const u16 data_02049f24[];
extern const u16 data_02049f70[];
extern const u16 data_02049ef4[];
extern const u16 data_02049f3c[];
extern const u16 data_02049f58[];
extern const u16 data_02049f64[];
extern const u16 data_02049f40[];
extern const u16 data_02049efc[];
extern const u16 data_02049f48[];
extern const u16 data_02049f34[];
extern const u16 data_02049f50[];
extern const u16 data_02049f20[];
extern const u16 data_02049f04;
extern const u16 data_02049f18;
extern const u16 data_02049f10;
extern const u16 data_02049f08;
extern const u16 data_02049f0c;
extern const u16 data_02049f1c;
extern const u16 data_02049f00;
extern const u16 data_02049ef8;
extern const u16 data_02049f38;
extern const u16 data_02049f30;
extern const u16 data_02049f28;
extern const u16 data_02049f14;

void GameAudio_LoadMapWaves(s16 map, u32 bytes_per_frame)
{
    u16 count = 0;
    const u16 *additional;
    u16 additional_count;
    count += data_02049f2c;
    MIi_CpuCopy16(data_02049f80, data_0205ac96, count * 2);
    additional = 0;
    additional_count = 0;
    switch (map & 0xffffu) {
    case 0x5001:
        additional = data_02049efc;
        additional_count = data_02049ef8;
        break;
    case 0x7009:
    case 0x700a:
    case 0x700b:
    case 0x700f:
        additional = data_02049f34;
        additional_count = data_02049f30;
        break;
    case 0x8008:
        additional = data_02049f50;
        additional_count = data_02049f28;
        break;
    case 0x201b:
        additional = data_02049ef4;
        additional_count = data_02049f10;
        break;
    case 0x2027:
        additional = data_02049f3c;
        additional_count = data_02049f08;
        break;
    case 0x2028:
        additional = data_02049f58;
        additional_count = data_02049f0c;
        break;
    case 0x2000:
        additional = data_02049f24;
        additional_count = data_02049f04;
        break;
    case 0x2012:
        additional = data_02049f70;
        additional_count = data_02049f18;
        break;
    case 0x2029:
        additional = data_02049f64;
        additional_count = data_02049f1c;
        break;
    case 0x5000:
        additional = data_02049f40;
        additional_count = data_02049f00;
        break;
    case 0x600f:
        additional = data_02049f48;
        additional_count = data_02049f38;
        break;
    case 0x9016:
        additional = data_02049f20;
        additional_count = data_02049f14;
        break;
    }
    if (additional) {
        MIi_CpuCopy16(additional, data_0205ac96 + count, additional_count * 2);
        count += additional_count;
    }
    data_0205ac94 = count;
    GameAudio_LoadWaveList((const GameAudioWaveList *)&data_0205ac94, 1, bytes_per_frame);
}

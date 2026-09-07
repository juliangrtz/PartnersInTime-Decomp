#include <nitro.h>

/* Overlay-5 item release: mark the item idle and return it to its pool. */
typedef struct Overlay5ItemPool Overlay5ItemPool;
extern Overlay5ItemPool data_ov005_0206aa18;
extern void func_ov005_020697dc(Overlay5ItemPool *pool, void *item);

void func_ov005_020695cc(void *item) {
    ((u8 *)item)[0x30] = 0;
    func_ov005_020697dc(&data_ov005_0206aa18, item);
}

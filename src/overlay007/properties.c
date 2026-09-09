#include <game/scene_vm_state.h>
#include <nitro.h>

/* The scene block; only a few of its fields are typed here. */
extern u8 data_ov007_020905f0[];


void func_ov007_02088454(u32 selector, u16 value)
{
    if (selector == 5) *(u16 *)&data_ov007_020905f0[0x24] = value;
}

int func_ov007_020883d0(u32 selector)
{
    switch (selector) {
    case 3:
    case 4:
        return SceneMenu_GetSelectedItem();
    case 5:
        return *(s16 *)&data_ov007_020905f0[0x24];
    case 9:
        return 0x38;
    }
    return 0;
}

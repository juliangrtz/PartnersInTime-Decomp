#include <nitro.h>
#include <nitro/tp.h>

/* Touch state for this frame: the samples read from the auto-sampling buffer,
   the derived button-like flags and the last valid position. */
typedef struct Overlay5TouchState {
    TpData points[10];
    u8 count;
    u8 held;
    u8 repeat;
    u8 pressed;
    u16 x, y;
    u8 previous;
    u8 reserved59;
    u16 last_x, last_y;
} Overlay5TouchState;

typedef char Overlay5TouchStateSizeCheck[sizeof(Overlay5TouchState) == 0x5e ? 1 : -1];

extern Overlay5TouchState data_ov005_0206a36c;
extern TpData data_ov005_0206a324[9];
extern u8 data_ov005_0206a318, data_ov005_0206a31c, data_ov005_0206a320;

extern void func_ov005_020686cc(void);

void func_ov005_02068594(void) {
    u16 x, y;
    int held;
    u16 touch, validity;
    u8 previous;
    func_ov005_020686cc();
    if (data_ov005_0206a36c.count == 0) {
        x = 0;
        y = 0;
        touch = 0;
        validity = 0;
    } else {
        const TpData *sample = &data_ov005_0206a36c.points[data_ov005_0206a36c.count - 1];
        x = data_ov005_0206a36c.points[data_ov005_0206a36c.count - 1].x;
        y = sample->y;
        touch = sample->touch;
        validity = sample->validity;
    }
    if (touch != 0 && validity == 0) held = 1;
    else held = 0;
    previous = data_ov005_0206a36c.previous;
    data_ov005_0206a36c.held = held;
    data_ov005_0206a36c.pressed = ~previous & held;
    data_ov005_0206a36c.repeat = 0;
    if (data_ov005_0206a36c.held) {
        if (!(data_ov005_0206a320 & 0x7f)) {
            if (data_ov005_0206a320 & 0x80) data_ov005_0206a320 = data_ov005_0206a31c | 0x80;
            else data_ov005_0206a320 = data_ov005_0206a318 | 0x80;
            data_ov005_0206a36c.repeat = data_ov005_0206a36c.held;
        }
        --data_ov005_0206a320;
    } else {
        data_ov005_0206a320 = 0;
    }
    data_ov005_0206a36c.previous = data_ov005_0206a36c.held;
    if (!data_ov005_0206a36c.held) {
        data_ov005_0206a36c.x = data_ov005_0206a36c.last_x;
        data_ov005_0206a36c.y = data_ov005_0206a36c.last_y;
        return;
    }
    data_ov005_0206a36c.x = x;
    data_ov005_0206a36c.y = y;
    data_ov005_0206a36c.last_x = data_ov005_0206a36c.x;
    data_ov005_0206a36c.last_y = data_ov005_0206a36c.y;
}

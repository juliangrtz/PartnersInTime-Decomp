#include <nitro.h>

typedef struct Overlay7Point Overlay7Point;
typedef struct Overlay7Motion Overlay7Motion;

/* A tracked point: its current position, the position it held last frame and
   the position the interface draws it at. */
struct Overlay7Point {
    Overlay7Point *next;
    s16 x, y, z;
    s16 previous_x, previous_y, previous_z;
    s16 drawn_x, drawn_y, drawn_z;
    s16 reserved16;
    s16 distance;
};

/* A running motion. The trailing record is either a quadratic curve or the
   step the linear updates add each frame. */
struct Overlay7Motion {
    u8 unknown_00[4];
    s32 time;
    s16 duration;
    u8 unknown_0a[14];
    union {
        struct { s32 acceleration, speed; s16 origin, target; } curve;
        struct { s16 x, y, z; } step;
    } data;
};

typedef struct Overlay7MotionStep { s16 x, y, z; } Overlay7MotionStep;
typedef struct Overlay7MotionCurve {
    s32 acceleration, speed;
    s16 origin, target;
} Overlay7MotionCurve;

enum {
    REG_SQRTCNT = 0x040002b0,
    REG_SQRT_RESULT = 0x040002b4,
    REG_SQRT_PARAM = 0x040002b8
};

extern Overlay7Point *data_ov007_020a6b94;

extern Overlay7MotionStep *func_ov007_02086f74(Overlay7Point *point, int owner, int duration,
    void (*update)(Overlay7Point *point, Overlay7Motion *motion));

void func_ov007_02086c64(Overlay7Point *point, int dx, int dy, int dz);
void func_ov007_02086cbc(Overlay7Point *point, int dx, int dy, int dz);
void func_ov007_02086d08(Overlay7Point *point, int dx, int dy, int dz);
void func_ov007_02086a34(Overlay7Point *point, Overlay7Motion *motion);
void func_ov007_02086b58(Overlay7Point *point, Overlay7Motion *motion);

void func_ov007_02086d08(Overlay7Point *point, int dx, int dy, int dz) {
    Overlay7Point *node = data_ov007_020a6b94;
    for (;;) {
        if (!node) {
            func_ov007_02086c64(point, dx, dy, dz);
            return;
        }
        if (node == point) {
            func_ov007_02086cbc(point, dx, dy, dz);
            return;
        }
        node = node->next;
    }
}

void func_ov007_02086cbc(Overlay7Point *point, int dx, int dy, int dz) {
    point->x = point->x + dx;
    point->y = point->y + dy;
    point->z = point->z + dz;
    point->drawn_x = point->drawn_x + dx;
    point->drawn_y = point->drawn_y + dy;
    point->drawn_z = point->drawn_z + dz;
}

void func_ov007_02086c64(Overlay7Point *point, int dx, int dy, int dz) {
    point->previous_x = point->x;
    point->previous_y = point->y;
    point->previous_z = point->z;
    point->x = point->x + dx;
    point->y = point->y + dy;
    point->z = point->z + dz;
    point->drawn_x = point->x;
    point->drawn_y = point->y;
    point->drawn_z = point->z;
}

void func_ov007_02086bd4(Overlay7Point *point) {
    int dx = point->x - point->previous_x;
    int dy = point->y - point->previous_y;
    int dz = point->z - point->previous_z;
    *(vu16 *)REG_SQRTCNT = 0;
    *(vu32 *)REG_SQRT_PARAM = (dx * dx + dy * dy + dz * dz) << 16;
    while (*(vu16 *)REG_SQRTCNT & 0x8000) {
    }
    point->distance = (point->distance + (s32)*(vu32 *)REG_SQRT_RESULT) / 2;
}

void func_ov007_02086b58(Overlay7Point *point, Overlay7Motion *motion) {
    Overlay7MotionStep *step = (Overlay7MotionStep *)&motion->data;
    int t = (motion->time << 4) / motion->duration;
    point->x = point->x + step->x * t / 4096;
    point->y = point->y + step->y * t / 4096;
    point->z = point->z + step->z * t / 4096;
}

void func_ov007_02086abc(Overlay7Point *point, int owner, int dx, int dy, int dz,
                         int duration) {
    Overlay7MotionStep *step;
    if (duration <= 0) {
        point->x = point->x + dx;
        point->y = point->y + dy;
        point->z = point->z + dz;
        point->drawn_x = point->drawn_x + dx;
        point->drawn_y = point->drawn_y + dy;
        point->drawn_z = point->drawn_z + dz;
        return;
    }
    step = func_ov007_02086f74(point, owner, duration, func_ov007_02086b58);
    step->x = dx;
    step->y = dy;
    step->z = dz;
}

void func_ov007_02086a34(Overlay7Point *point, Overlay7Motion *motion) {
    Overlay7MotionStep *step = (Overlay7MotionStep *)&motion->data;
    int t = (motion->time << 4) / motion->duration;
    point->x = point->x + (step->x - point->x) * t / 4096;
    point->y = point->y + (step->y - point->y) * t / 4096;
    point->z = point->z + (step->z - point->z) * t / 4096;
}

void func_ov007_020869c4(Overlay7Point *point, int owner, int x, int y, int z,
                         int duration) {
    Overlay7MotionStep *step;
    if (duration <= 0) {
        func_ov007_02086d08(point, x - point->x, y - point->y, z - point->z);
        return;
    }
    step = func_ov007_02086f74(point, owner, duration, func_ov007_02086a34);
    step->x = x;
    step->y = y;
    step->z = z;
}

void func_ov007_02086974(Overlay7Point *point, Overlay7Motion *motion) {
    Overlay7MotionCurve *curve = (Overlay7MotionCurve *)&motion->data;
    int t = motion->time / 256;
    if (t == motion->duration) {
        point->z = curve->target;
        return;
    }
    point->z = curve->origin + (curve->acceleration * t + curve->speed) * t / 4096;
}

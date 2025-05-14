    #ifndef HARDWARE_H
    #define HARDWARE_H

#include <nitro.h>

#define rDIVCNT             ((vu16*)0x04000280)
#define DIVCNTB_BUSY        15
#define DIVCNTF_BUSY        (0b1 << DIVCNTB_BUSY)
#define DIVCNTB_DIV0        14
#define DIVCNTF_DIV0        (0b1 << DIVCNTB_DIV0)
#define DIVCNTB_MODE        0
#define DIVCNTF_MODE        (0b11 << DIVCNTB_MODE)
#define DIVCNT_MODE_32_32   (0 << DIVCNTB_MODE)
#define DIVCNT_MODE_64_32   (1 << DIVCNTB_MODE)
#define DIVCNT_MODE_64_64   (2 << DIVCNTB_MODE)

#define rDIV_NUMER          ((vu64*)0x04000290)
#define rDIV_NUMER_L        ((vu32*)0x04000290)
#define rDIV_NUMER_H        ((vu32*)0x04000294)

#define rDIV_DENOM          ((vu64*)0x04000298)
#define rDIV_DENOM_L        ((vu32*)0x04000298)
#define rDIV_DENOM_H        ((vu32*)0x0400029C)

#define rDIV_RESULT         ((vu64*)0x040002A0)
#define rDIV_RESULT_L       ((vu32*)0x040002A0)
#define rDIV_RESULT_H       ((vu32*)0x040002A4)

#define rDIVREM_RESULT      ((vu64*)0x040002A8)
#define rDIVREM_RESULT_L    ((vu32*)0x040002A8)
#define rDIVREM_RESULT_H    ((vu32*)0x040002AC)

#define rSQRTCNT            ((vu16*)0x040002B0)
#define SQRTCNTB_BUSY       15
#define SQRTCNTF_BUSY       (0b1 << SQRTCNTB_BUSY)
#define SQRTCNTB_MODE       0
#define SQRTCNTF_MODE       (0b1 << SQRTCNTB_MODE)
#define SQRTCNT_MODE_32     (0 << SQRTCNTB_MODE)
#define SQRTCNT_MODE_64     (1 << SQRTCNTB_MODE)

#define rSQRT_RESULT        ((vu32*)0x040002B4)

#define rSQRT_PARAM         ((vu64*)0x040002B8)
#define rSQRT_PARAM_L       ((vu32*)0x040002B8)
#define rSQRT_PARAM_H       ((vu32*)0x040002BC)

    #endif

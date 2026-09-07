#ifndef NITRO_OS_OWNER_H
#define NITRO_OS_OWNER_H
#include <nitro.h>

typedef struct OsOwnerInfo {
    u8 language, favorite_color, birthday_month, birthday_day;
    u16 nickname[10];
    u16 nickname_length;
    u16 comment[26];
    u16 comment_length;
} OsOwnerInfo;
typedef char OsOwnerInfo_SizeCheck[sizeof(OsOwnerInfo) == 80 ? 1 : -1];

#ifdef __cplusplus
extern "C" {
#endif
void OS_GetOwnerInfo(OsOwnerInfo *owner);
void OS_GetMacAddress(u8 *address);
u64 OS_GetOwnerValue68(void);
#ifdef __cplusplus
}
#endif
#endif

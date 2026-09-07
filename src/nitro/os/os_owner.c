#include <nitro/os_owner.h>

extern void MIi_CpuCopy16(const void *, void *, u32);
extern void MI_CpuCopy8(const void *, void *, u32);

/* Prefix of the settings copied by firmware into shared RAM. */
typedef struct OsOwnerSettingsPrefix {
    u8 unknown_00[2];
    u8 favorite_color : 4, unknown_color : 4;
    u8 birthday_month, birthday_day, unknown_05;
    u16 nickname[10];
    u8 nickname_length, unknown_1b;
    u16 comment[26];
    u8 comment_length, unknown_51[19];
    u16 language : 3, unknown_language : 13;
    u16 unknown_66;
    u64 unknown_68;
} OsOwnerSettingsPrefix;
typedef char OsOwnerSettingsPrefix_SizeCheck[sizeof(OsOwnerSettingsPrefix) == 112 ? 1 : -1];

void OS_GetMacAddress(u8 *address)
{
    MI_CpuCopy8((const void *)0x027ffcf4, address, 6);
}

void OS_GetOwnerInfo(OsOwnerInfo *owner)
{
    const OsOwnerSettingsPrefix *settings = (const OsOwnerSettingsPrefix *)0x027ffc80;
    owner->language = settings->language;
    owner->favorite_color = settings->favorite_color;
    owner->birthday_month = settings->birthday_month;
    owner->birthday_day = settings->birthday_day;
    owner->nickname_length = settings->nickname_length;
    owner->comment_length = settings->comment_length;
    MIi_CpuCopy16(settings->nickname, owner->nickname, sizeof(owner->nickname));
    MIi_CpuCopy16(settings->comment, owner->comment, sizeof(owner->comment));
}

#ifndef PIT_GAME_OVERLAY005_POOLS_H
#define PIT_GAME_OVERLAY005_POOLS_H

#include <nitro.h>

/* Pools share a layout but retain distinct element and link types. */

struct Overlay5Element;
struct Overlay5ElementLink;
typedef struct Overlay5ElementPool {
    struct Overlay5Element *elements;
    struct Overlay5ElementLink *links;
    struct Overlay5ElementLink *free_list;
    struct Overlay5ElementLink *taken_list;
    u32 count;
} Overlay5ElementPool;
typedef char Overlay5ElementPool_SizeCheck[sizeof(Overlay5ElementPool) == 20 ? 1 : -1];

struct Overlay5Sprite;
struct Overlay5SpriteNode;
typedef struct Overlay5SpritePool {
    struct Overlay5Sprite *sprites;
    struct Overlay5SpriteNode *nodes;
    struct Overlay5SpriteNode *free_list;
    struct Overlay5SpriteNode *taken_list;
    u32 count;
} Overlay5SpritePool;
typedef char Overlay5SpritePool_SizeCheck[sizeof(Overlay5SpritePool) == 20 ? 1 : -1];

struct Overlay5DrawNode;
struct Overlay5DrawLink;
typedef struct Overlay5DrawPool {
    struct Overlay5DrawNode *entries;
    struct Overlay5DrawLink *links;
    struct Overlay5DrawLink *free_list;
    struct Overlay5DrawLink *taken_list;
    u32 count;
} Overlay5DrawPool;
typedef char Overlay5DrawPool_SizeCheck[sizeof(Overlay5DrawPool) == 20 ? 1 : -1];

struct Overlay5Item;
struct Overlay5ItemLink;
typedef struct Overlay5ItemPool {
    struct Overlay5Item *items;
    struct Overlay5ItemLink *links;
    struct Overlay5ItemLink *free_list;
    struct Overlay5ItemLink *taken_list;
    u32 count;
} Overlay5ItemPool;
typedef char Overlay5ItemPool_SizeCheck[sizeof(Overlay5ItemPool) == 20 ? 1 : -1];

#endif

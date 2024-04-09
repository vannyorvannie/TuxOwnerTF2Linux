#ifndef SDK_STUDIOHDR_H_
#define SDK_STUDIOHDR_H_
#include "../SDK.h"

/* NOTE: This file is included from sdk.h and depends on its declarations */
typedef float matrix3x4[3][4];

enum ETFHitbox {
    /* Head */
    HITBOX_HEAD,

    /* Torso */
    HITBOX_PELVIS,
    HITBOX_SPINE0,
    HITBOX_SPINE1,
    HITBOX_SPINE2,
    HITBOX_SPINE3,

    /* Arms */
    HITBOX_LEFT_UPPER_ARM,
    HITBOX_LEFT_LOWER_ARM,
    HITBOX_LEFT_HAND,
    HITBOX_RIGHT_UPPER_ARM,
    HITBOX_RIGHT_LOWER_ARM,
    HITBOX_RIGHT_HAND,

    /* Legs */
    HITBOX_LEFT_HIP,
    HITBOX_LEFT_KNEE,
    HITBOX_LEFT_FOOT,
    HITBOX_RIGHT_HIP,
    HITBOX_RIGHT_KNEE,
    HITBOX_RIGHT_FOOT,

    HITBOX_MAX,
};

typedef struct {
    int bone;
    int group;
    Vector bbmin;
    Vector bbmax;
    int szhitboxnameindex;
    int unused[8];
} studiobbox_t;

typedef struct {
    int sznameindex;
    /* pszName */
    int numhitboxes;
    int hitboxindex;
    /* pHitbox */
} studiohitboxset_t;

static inline studiobbox_t* studiohitboxset_pHitbox(studiohitboxset_t* thisptr,
                                                    int i) {
    return (studiobbox_t*)(((void*)thisptr) + thisptr->hitboxindex) + i;
};

typedef struct {
    int sznameindex;
    int parent;
    int bonecontroller[6];
    Vector pos;
    Vector quat;
    Vector rot; /* RadianEuler */
    Vector posscale;
    Vector rotscale;
    matrix3x4 poseToBone;
    Vector qAlignment;
    int flags;
    int proctype;
    int procindex;
    int physicsbone;
    int surfacepropidx;
    int contents;
    int unused[8];
} studiobone_t;

struct studiohdr_t {
    int id;
    int version;
    int checksum;
    /* pszName() */
    char name[64];
    int length;
    Vector eyeposition;
    Vector illumposition;
    Vector hull_min;
    Vector hull_max;
    Vector view_bbmin;
    Vector view_bbmax;
    int flags;
    int numbones;
    int boneindex;
    /* pBone(int i) */
    int numbonecontrollers;
    int bonecontrollerindex;
    int numhitboxsets;
    int hitboxsetindex;
    /* ... */
};

static inline studiobone_t* studiohdr_pBone(studiohdr_t* thisptr, const int i) {
    if (i < 0 || i >= thisptr->numbones)
        return NULL;

    return (studiobone_t*)(((void*)thisptr) + thisptr->boneindex) + i;
}

static inline studiohitboxset_t* studiohdr_pHitboxSet(studiohdr_t* thisptr,
                                                      int i) {
    if (i < 0 || i >= thisptr->numhitboxsets)
        return NULL;

    return (studiohitboxset_t*)(((void*)thisptr) + thisptr->hitboxsetindex) + i;
}

/* Wrapper for studiohdr_pHitboxSet and studiohitboxset_pHitbox */
static inline studiobbox_t* studiohdr_pHitbox(studiohdr_t* thisptr, int set,
                                              int idx) {
    studiohitboxset_t* hitboxset = studiohdr_pHitboxSet(thisptr, set);
    if (!hitboxset)
        return NULL;

    return (studiobbox_t*)studiohitboxset_pHitbox(hitboxset, idx);
}

#endif /* SDK_STUDIOHDR_H_ */
#include "../../SDK.h"
#include "../../SDK/studiohdrfrom8dcc.h"
#include "../../SDK/bspflags.h"
#define VEC_ZERO ((Vector){ 0.f, 0.f, 0.f })
#define HITBOX_SET 0
#define MAXSTUDIOBONES 128


class MiscAim
{
public:
    float GetFOV(Vector angle, Vector src, Vector dst);
    void MakeVector(Vector angle, Vector& vector);
    Vector calc_angle(Vector src, Vector dst);
    float flGetDistance(Vector vOrigin, Vector vLocalOrigin);
    void silentMovementFix(CUserCmd *pUserCmd, Vector angles); //Fix for silent movement    creds gir + f1ssi0n
    void FixMovementForUserCmd(CUserCmd* cmd, old_movement_t mov); // old, from potassium.
    bool IsVisible(void* pLocal, void* pEntity, Vector vStart, Vector vEnd);
};

extern MiscAim gMiscAim;
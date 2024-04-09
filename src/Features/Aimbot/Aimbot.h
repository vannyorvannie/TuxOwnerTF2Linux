#include "../../SDK.h"
#include "../../SDK/studiohdrfrom8dcc.h"
#include "MiscAimbot.h"
class CAim
{
public:
    void setting_to_hitboxes(int setting, int* min, int* max);
    int GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity);
    int GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand);
    void Run(CBaseEntity* pLocal, CUserCmd* pCommand);
};

extern CAim gAim;
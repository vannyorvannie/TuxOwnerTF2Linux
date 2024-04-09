#include "../../SDK.h"
#include "../../CDrawManager.h"
class ESP
{
public:
    void StartThisshit(CBaseEntity* pLocal);
    void ActualfreakinESP(CBaseEntity* pEntity);
    //void ESPLOCAL(CBaseEntity* pEntity);
    int  GetPlayerColor(CBaseEntity* pPlayer);
    void Buildings(CBaseEntity* pEntity);
    void DrawBone(CBaseEntity* pEntity, int* iBones, int count, int clrCol);
};

extern ESP gESP;
#include "../../SDK.h"

class CMisc
{
public:
    void Run(CBaseEntity* pLocal, CUserCmd* pCmd);
    void BHOP(CUserCmd* pCmd);
    void AutoStrafe(CUserCmd* pCmd);
    void SpeedCrouch(CUserCmd* pCmd);
	void TauntSpin(CUserCmd* pCmd, CBaseEntity* pLocal);
};

extern CMisc gMisc;

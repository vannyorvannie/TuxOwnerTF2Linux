#pragma once
#include "../../SDK.h"
#include "../../Client.h"
#include "../../CMenu.h"
class CHvH
{
public:

	void Run(CBaseEntity* pLocal, CUserCmd* pCmd);
};

extern CHvH gHvH;
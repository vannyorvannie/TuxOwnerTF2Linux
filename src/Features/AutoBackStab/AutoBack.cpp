#include "AutoBack.h"
#include "../../CMenu.h"
CAutoBackStab gStab;
/*
* ToDO: Priorities
*/

void CAutoBackStab::Run(CBaseEntity* pLocal, CUserCmd* pCmd)
{
    if (!gCheatMenu.misc_autobackstab)
        return;
    
	if (!pLocal->GetActiveWeapon())
    {
        return;
    }
   // if (pLocal->szGetClass() != "Spy")
   ///     return;
    if (pLocal->GetActiveWeapon()->GetWeaponID() == TF_WEAPON_PDA_SPY_BUILD || pLocal->GetActiveWeapon()->GetWeaponID() == TF_WEAPON_REVOLVER)
        return;
    if (pLocal->GetActiveWeapon()->GetWeaponID() == TF_WEAPON_PDA_SPY)
        return;
    if (pLocal->GetActiveWeapon()->IsBackstabable())
    {
        pCmd->buttons |= IN_ATTACK;
    }
}




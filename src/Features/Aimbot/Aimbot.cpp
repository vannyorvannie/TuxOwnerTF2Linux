#include "Aimbot.h"
#include "../../SDK/studiohdrfrom8dcc.h"
#include "../../CMenu.h"
#include "../../SDK/bspflags.h"
#include <SDL2/SDL.h>

/*
*   Original Coder of the studiohdr & gethitbox: 8dcc.
*
*   Ported to TuxOwner by Vannie
*           13-01-2024
*/

CAim gAim;

int CAim::GetBestTarget(CBaseEntity* pLocal, CUserCmd* pCommand)
{
	int iBestTarget = -1;
	float flDistToBest = 99999.f;

	Vector vLocal = pLocal->GetEyePosition();

	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive() || pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;

		int iBestHitbox = GetBestHitbox(pLocal, pEntity);

		if (iBestHitbox == -1)
			continue;

		Vector vEntity = pEntity->get_hitbox_pos(iBestHitbox); //pEntity->GetWorldSpaceCenter(vEntity);

		if (!gCheatMenu.PlayerMode[i])
			continue;

		if (pEntity->GetCond() & TFCond_Ubercharged ||
			//pEntity->GetCond() & TFCond_UberchargeFading || <- ubercharge fading ignores forever for some reason. Get rid of it!
			pEntity->GetCond() & TFCond_Bonked)
			continue;
        if (pEntity->GetCond() & TFCond_Cloaked && gCheatMenu.priorties_ignorecloaked && gCheatMenu.PlayerMode[i] != 2)
        {
            continue;
        }
        if (pEntity->GetCond() & TFCond_Taunting && gCheatMenu.priorties_ignoretaunting && gCheatMenu.PlayerMode[i] != 2)
        {
            continue;
        }
        if (pEntity->GetCond() & TFCond_Disguised && gCheatMenu.priorties_ignoredisguised && gCheatMenu.PlayerMode[i] != 2)
        {
            continue;
        }
		

		float flDistToTarget = (vLocal - vEntity).Length();
		double minimalDistance;
		if (gCheatMenu.aimbot_infinitedistance)
        	minimalDistance = 99999.0;
		else
			minimalDistance = gCheatMenu.aimbot_distance;
        float flFOV = gMiscAim.GetFOV(pCommand->viewangles, vLocal, vEntity);
		float distance = gMiscAim.flGetDistance(vEntity, pLocal->GetEyePosition());
		if (distance < minimalDistance)//gCvars.aimbot.fov)
		{
	
			if (flFOV < gCheatMenu.aimbot_fov)
			{
				if (gCheatMenu.PlayerMode[i] == 2)
					return i;
				if (pEntity->GetHealth() < 100 && gCheatMenu.aimbot_preferlowhp)
					return i;
				if (pEntity->GetClassNum() == TF2_Heavy && gCheatMenu.priorties_preferheavy)
					return i;
				if (pEntity->GetClassNum() == TF2_Medic && gCheatMenu.priorties_prefermedic)
					return i;
				gCheatMenu.iAimbotIndex = i;
				iBestTarget = i;
			}
		}
	}

	return iBestTarget;
}

int CAim::GetBestHitbox(CBaseEntity* pLocal, CBaseEntity* pEntity)
{
	int iBestHitbox = -1;

	if (!gCheatMenu.aimbot_hitbox)
	{
		if (pLocal->GetClassNum() == TF2_Sniper)
			iBestHitbox = 0;
		else
			iBestHitbox = 4;
	}
	else
	{
		iBestHitbox = gCheatMenu.aimbot_hitbox - 1;
	}

	if (gCheatMenu.aimbot_hitscan)
	{
		for (int i = 0; i < 17; i++)
		{
			if (gMiscAim.IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->get_hitbox_pos(i)))
				return i;
		}
	}

	if (pEntity->get_hitbox_pos(iBestHitbox) == VEC_ZERO)
		return -1;

	if (!gMiscAim.IsVisible(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->get_hitbox_pos(iBestHitbox)))
		return -1;
	
	return iBestHitbox;
}

void CAim::Run(CBaseEntity* pLocal, CUserCmd* pCommand)
{    

	const Uint8 *epic = SDL_GetKeyboardState(NULL);


	old_movement_t old_mov = old_movement_t();
    // i genuinely think get slot is fucked lmao
	if (!pLocal->GetActiveWeapon())
		return;
	
	
	if (pLocal->GetActiveWeapon()->GetWeaponID() == TF_WEAPON_KNIFE)
		return;  
	if (gCheatMenu.aimbot_silent) { // only backup the c_usercmd data when it's needed.
		old_mov.angle = pCommand->viewangles;
		old_mov.fwd = pCommand->forwardmove;
		old_mov.sdm = pCommand->sidemove;
	}

	gCheatMenu.iAimbotIndex = -1;

	if (!gCheatMenu.aimbot_active)
		return;
	
	if (pCommand->buttons & IN_RELOAD) // if we are reloading, fucking stop trying to shoot
	{
		return;
	}
	CBaseEntity* pEntity = GetBaseEntity(GetBestTarget(pLocal, pCommand));

	if (!pEntity)
		return;

	int iBestHitbox = GetBestHitbox(pLocal, pEntity);

	if (iBestHitbox == -1)
		return;

	Vector vEntity = pEntity->get_hitbox_pos(iBestHitbox);

	Vector vLocal = pLocal->GetEyePosition();

	Vector vAngs;
	Vector DIFF = vEntity - vLocal;
	VectorAngles(DIFF, vAngs);

	ClampAngle(vAngs);
	gCheatMenu.iAimbotIndex = pEntity->GetIndex();
	//auto pWep = pLocal->GetActiveWeapon();

	if (gCheatMenu.warp_enable && !epic[SDL_SCANCODE_R])
	{
		return;
	}

	pCommand->viewangles = vAngs; // always set this cuz otherwise the viewangles will desync.

	if (!gCheatMenu.aimbot_silent) {
		gInts.Engine->SetViewAngles(pCommand->viewangles);
	}

	if (gCheatMenu.aimbot_silent) 
    { // apply our movement fix if silent aim is enabled.
		gMiscAim.FixMovementForUserCmd(pCommand, old_mov);
	}
	
	if (gCheatMenu.aimbot_autoshoot)
	{
		float flCurTime = gInts.Engine->Time();
		static float flNextSend = 0.0f;
		if (gCheatMenu.aimbot_zoomedonly)
		{
			if (pLocal->IsScoped())
			{
				if (flCurTime > flNextSend)
				{
					pCommand->buttons |= IN_ATTACK;
					flNextSend = (flCurTime + 0.2f);
				}
			}
		}
		if (!gCheatMenu.aimbot_zoomedonly) // yey
		{
			pCommand->buttons |= IN_ATTACK;
		}
	}

	if (gCheatMenu.aimbot_autoscope && !pLocal->IsScoped() && pLocal->GetClassNum() == TF2_Sniper)
	{
		pCommand->buttons |= IN_ATTACK2;
	}
    
}
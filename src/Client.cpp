#include "SDK.h"
#include "Client.h"
#include "Features/Triggerbot/Triggerbotterino.h"
#include "Features/Aimbot/Aimbot.h"
#include "Features/Misc/Misc.h"
#include "Features/AutoBackStab/AutoBack.h"
#include "Features/autistvsautist/HvH.h"
#include "Followbot.h"
#include "CSignature.h"

CGlobalzz g;
Vector qLASTTICK;


//============================================================================================
bool Hooked_CreateMove(void *ClientMode, float input_sample_frametime, CUserCmd *pCommand)
{
	
	//If you want taunt slide, you will need to hook CHLClient::CreateMove and do it there. If you do it here, you'll just shimmy forward.
	VMTManager& hook = VMTManager::GetHook(ClientMode); //Get a pointer to the instance of your VMTManager with the function GetHook.
	bool bReturn = hook.GetMethod<bool(*)(void *, float, CUserCmd*)>(gOffsets.iCreateMoveOffset)(ClientMode, input_sample_frametime, pCommand); //Call the original.
	try
	{
		

		auto WalkTo = [&](const Vector &vFrom, const Vector &vTo, float flScale) -> void
		{	
			Vector vDelta = vTo - vFrom;

			if (vDelta.Length() == 0.0f)
				return;

			Vector vDeltaMove = { vDelta.x, vDelta.y, 0.0f }, vDeltaDir = {};
			VectorAngles(vDeltaMove, vDeltaDir);
			float flYaw = DEG2RAD(vDeltaDir.y - pCommand->viewangles.y);
			pCommand->forwardmove = cosf(flYaw) * (450.0f * flScale);
			pCommand->sidemove = -sinf(flYaw) * (450.0f * flScale);
		};

    // TODO: Move this to FrameStageNotify and cast it to a global variable
		CBaseEntity* pLocal = GetBaseEntity(me); //Grab the local player's entity pointer.

    // TODO: wrap this
		if (pLocal == NULL)
			return bReturn;

		CBaseEntity *oEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());		


		

		gTrigger.Run(pLocal, pCommand);
		gAim.Run(pLocal, pCommand);
		gMisc.Run(pLocal, pCommand);
		gStab.Run(pLocal, pCommand);
		gHvH.Run(pLocal, pCommand);
		gFollow.Run(pLocal, pCommand);

		qLASTTICK = pCommand->viewangles;

		if (gCheatMenu.isThirdPersonEnabled)
		{
			auto *m_angEyeAngles = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 8);
		
			auto *HTC = reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 4);

			*HTC = qLASTTICK.x;
			*m_angEyeAngles = qLASTTICK.y;
		}
	}
	catch(...)
	{
		// Log::Fatal("Failed Hooked_CreateMove");
	}
	//return bReturn; //If bReturn is true, CInput::CreateMove will call CEngine::SetViewAngles(pCommand->viewangles). If you want silent aim, return false, but make sure to call SetViewAngles manually.
	
	return false;
}


void FrameStageNotifyThink(void* CHLClient, void *_this, ClientFrameStage_t Stage)
{
	if (Stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (auto i = 1; i <= gInts.Engine->GetMaxClients(); i++)
		{
			CBaseEntity *entity = nullptr;
			player_info_t temp;

			if (!(entity = gInts.EntList->GetClientEntity(i)))
				continue;

			if (entity->IsDormant())
				continue;

			if (!gInts.Engine->GetPlayerInfo(i, &temp))
				continue;

			if (!entity->IsAlive())
				continue;

			Vector vX = entity->GetAngles();
			Vector vY = entity->GetAnglesHTC();
			auto *WritePitch = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]"));
			auto *WriteYaw = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[1]"));
			if (gCheatMenu.hvh_resolver)
			{
				if (vX.x == -89.0f)
				{
					*WritePitch = 90.0f;
				}
				if (vX.x == 89.0f)
				{
					*WritePitch = -90.0f;
				}
			}
		}
	}
	/*
	gESP.FrameStageNotify(Stage);

	if (Stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		if (GAME_TF2)
		{
			for (auto i = 1; i <= gInts.Engine->GetMaxClients(); i++)
			{
				CBaseEntity *entity = nullptr;
				player_info_t temp;

				if (!(entity = gInts.EntList->GetClientEntity(i)))
					continue;

				if (entity->IsDormant())
					continue;

				if (!gInts.Engine->GetPlayerInfo(i, &temp))
					continue;

				if (!entity->GetLifeState() == LIFE_ALIVE)
					continue;

				Vector vX = entity->GetAngles();
				Vector vY = entity->GetAnglesHTC();
				auto *WritePitch = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]"));
				auto *WriteYaw = reinterpret_cast<float*>(reinterpret_cast<DWORD>(entity) + gNetVars.get_offset("DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[1]"));

				bool resolver = false;

				if (Util->IsKeyPressedMisc(gCvars.aimbot_resolver_key))
				{
					resolver = !resolver;
				}
				if (gCvars.aimbot_resolver && resolver)
				{
					if (vX.x == -89.0f)
					{
						*WritePitch = 90.0f;
					}
					if (vX.x == 89.0f)
					{
						*WritePitch = -90.0f;
					}
				}
			}
		}
   }

	if (gInts.Engine->IsInGame() && Stage == FRAME_RENDER_START)
	{
			CBaseEntity *oEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

			int *Fov;
			int OldFov;

			int m_flFOVRate = 0xE5C;// Broken: nv.get_offset("DT_BasePlayer", "localdata", "m_flFOVRate");
			int &fovPtr = *(int*)(oEntity + gNetVars.get_offset("DT_BasePlayer", "m_iFOV")), defaultFov = *(int*)(oEntity + gNetVars.get_offset("DT_BasePlayer", "m_iDefaultFOV"));
			if (gESP.nozoom.value) //Thanks cademtz/Hold on! for this code, works amazingly aside from the inivisible sniper rifle xd
			{
				fovPtr = defaultFov;
				*(float*)(oEntity + m_flFOVRate) = 0;
			}

			if (gESP.thirdperson.value) //shows antiaims just fine
			{
				CBaseEntity *oEntity = gInts.EntList->GetClientEntity(gInts.Engine->GetLocalPlayer());

				auto *m_angEyeAngles = reinterpret_cast<float*>(reinterpret_cast<DWORD>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 8);

				auto *HTC = reinterpret_cast<float*>(reinterpret_cast<DWORD>(oEntity) + gNetVars.get_offset("DT_BasePlayer", "pl", "deadflag") + 4);

				*HTC = qLASTTICK.x;
				*m_angEyeAngles = qLASTTICK.y;

				ConVar* sv_cheats = gInts.cvar->FindVar("sv_cheats");
				if (sv_cheats->GetInt() == 0) sv_cheats->SetValue(1);
				ConVar* pThirdCamYaw = gInts.cvar->FindVar("cam_idealyaw");
				gInts.Engine->ClientCmd_Unrestricted("thirdperson");

				pThirdCamYaw->SetValue(0);
			}
			else if (!gESP.thirdperson.value)
			{
				gInts.Engine->ClientCmd_Unrestricted("firstperson");
			}
	}
	*/


	VMTManager &HTCCNKBRKYLC = VMTManager::GetHook(CHLClient);
	return HTCCNKBRKYLC.GetMethod<void(*)(void *, void *, ClientFrameStage_t)>(35)(CHLClient, _this, Stage);
}
//============================================================================================

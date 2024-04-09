#include "argh.h"
#include "CMenu.h"
#include <cstring>

#define GET_PLAYER_USERID(userid) gInts.EntList->GetClientEntity(gInts.Engine->GetPlayerForUserID(userid))

CKillSay gKillSay;
void CKillSay::InitKillSay()
{
	gInts.EventManager->AddListener(this, "player_death", false);
	gInts.EventManager->AddListener(this, "player_hurt", false);
	gInts.EventManager->AddListener(this, "player_disconnect", false);
	gInts.EventManager->AddListener(this, "player_changeclass", false);
	gInts.EventManager->AddListener(this, "vote_cast", false);
	wasEventInitialized = true;
}

inline const char* get_player_class( int class_id ) 
{
	switch ( class_id ) 
	{
		case TF2_Scout: { return "Scout"; }
		case TF2_Soldier: { return "Soldier"; }
		case TF2_Pyro: { return "Pyro"; }
		case TF2_Demoman: { return "Demoman"; }
		case TF2_Heavy: { return "Heavy"; }
		case TF2_Engineer: { return "Engineer"; }
		case TF2_Medic: { return "Medic"; }
		case TF2_Sniper: { return "Sniper"; }
		case TF2_Spy: { return "Spy"; }
		default: return "";
	}

	return "";
}

int GetPlayerForUserID(int userID)
{
	for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		player_info_t playerInfo;
		if (i == me)
			continue;

		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			return;
		if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &playerInfo))
		{
			continue;
		}
		// Found player
		if (playerInfo.userID == userID)
		{
			return i;
		}
	}
	return 0;
}

void CKillSay::FireGameEvent(IGameEvent *event)
{	
	if (!strcmp(event->GetName(), "player_changeclass")) 
	{
		if(const auto& pEntity = GET_PLAYER_USERID(event->GetInt("userid")))
		{
			//int nIndex = pEntity->GetIndex();
			player_info_t pInfo;

			auto class_str = get_player_class(event->GetInt("class"));

			if (gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
			{
				std::string actualstringforchangeclass = "\x6[\x3Tux\x6] \x4" + std::string(pInfo.name) + " changed class to " + std::string(class_str);  
		
				gInts.ClientMode->m_pChat->ChatPrintf(0, actualstringforchangeclass.c_str());	  
			}
		}
	}
}
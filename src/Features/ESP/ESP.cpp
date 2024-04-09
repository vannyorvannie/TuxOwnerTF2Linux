#include "ESP.h"
#include "../../CMenu.h"
#include "../../Radar.h"
ESP gESP;
void ESP::StartThisshit(CBaseEntity* pLocal)
{
	//ESPLOCAL(pLocal); // run the local esp outside of pEnt loop
    for (int i = 1; i <= gInts.EntList->GetHighestEntityIndex(); i++)
	{
		if (i == me)
			continue;
		if (!gCheatMenu.esp_active)
			continue;
		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;

		if (pEntity->IsDormant())
			continue;

		if (!pEntity->IsAlive())
			continue;
		if (gCheatMenu.esp_enemyonly && pEntity->GetTeamNum() == pLocal->GetTeamNum())
			continue;		
		if (gCheatMenu.esp_active)
			Buildings(pEntity);	
			ActualfreakinESP(pEntity);
	}
}

void ESP::Buildings(CBaseEntity* pEntity) /* do other buildings later */
{
	switch (pEntity->GetClientClass()->iClassID)
	{
	case CObjectSentrygun:
	{
		CTFObjectSentryGun* pSentryGun = reinterpret_cast<CTFObjectSentryGun*>(pEntity);

		if (!pSentryGun)
			return;

		Vector vecWorld, vecScreen; //Setup the Vectors.

		pSentryGun->GetWorldSpaceCenter(vecWorld); //Get the center of the player.

		if (gDrawManager.WorldToScreen(vecWorld, vecScreen) && pSentryGun && pSentryGun->IsAlive()) //If the player is visble.
		{
			gDrawManager.DrawString(vecScreen.x, vecScreen.y, COLORCODE(255,255,255,255), "Sentry"); //Draw on the player.
			if (!pSentryGun->IsBuilding())
			{
				gDrawManager.DrawString(vecScreen.x, vecScreen.y + 10, 0xFFFFFFFF, "Level: %i", pSentryGun->GetLevel()); //Draw on the player.
			}
		}
	}
	break;
	case CObjectDispenser:
	{
		CTFObjectDispenser* pDispenser = reinterpret_cast<CTFObjectDispenser*>(pEntity);

		if (!pDispenser)
			return;

		//int teamcolor = GetPlayerColor(pDispenser);

		Vector vecWorld, vecScreen;

		pDispenser->GetWorldSpaceCenter(vecWorld);

		if (gDrawManager.WorldToScreen(vecWorld, vecScreen) && pDispenser && pDispenser->IsAlive())
		{
			gDrawManager.DrawString(vecScreen.x, vecScreen.y, COLORCODE(255,255,255,255), "Dispenser"); //Draw on the player.
			if (!pDispenser->IsBuilding())
			{
				gDrawManager.DrawString(vecScreen.x, vecScreen.y + 10, 0xFFFFFFFF, "Level: %i", pDispenser->GetLevel()); //Draw on the player.
			}
		}
	}
	break;
	}
}

int ESP::GetPlayerColor(CBaseEntity* pPlayer)
{
	if (gCheatMenu.PlayerMode[pPlayer->GetIndex()] == 2)
	{
		return COLORCODE(75,0,130,255);
	}
	if (!gCheatMenu.PlayerMode[pPlayer->GetIndex()]) // ignored
	{
		return COLORCODE(15,150,150,255); // NCC colors
	}
	if (pPlayer->GetIndex() == gCheatMenu.iAimbotIndex)
		return COLORCODE(0, 255, 0, 255); // green
	switch (pPlayer->GetTeamNum())
	{
	case 2: //RED
		return COLORCODE(255, 20, 20, 255);
	case 3: //BLU
		return COLORCODE(0, 153, 255, 255);
	default:
		return COLORCODE(0, 153, 255, 255);
	}
	//return Color(0, 0, 0, 0); //no reason for this to be here, i just wanna look smart
}


/*
TODO:
Team Color. <- DONE!
Conditions. <- DONE!
Rewrite the esp to look more NullCore's ESP.
Yes. Nullcore's.

*/
void ESP::ActualfreakinESP(CBaseEntity* pEntity)
{
	CBaseEntity* pLocalEntity = GetBaseEntity(me);

	if (pLocalEntity == NULL)
		return;
	int CONDCOLOR = COLORCODE(255, 255, 0, 255);
    player_info_t pInfo;
	if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
		return;

	const matrix3x4& vMatrix = pEntity->GetRgflCoordinateFrame();

	Vector vMin = pEntity->GetCollideableMins();
	Vector vMax = pEntity->GetCollideableMaxs();

	Vector vPointList[] = {
		Vector(vMin.x, vMin.y, vMin.z),
		Vector(vMin.x, vMax.y, vMin.z),
		Vector(vMax.x, vMax.y, vMin.z),
		Vector(vMax.x, vMin.y, vMin.z),
		Vector(vMax.x, vMax.y, vMax.z),
		Vector(vMin.x, vMax.y, vMax.z),
		Vector(vMin.x, vMin.y, vMax.z),
		Vector(vMax.x, vMin.y, vMax.z)
	};

	Vector vTransformed[8];

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 3; j++)
			vTransformed[i][j] = vPointList[i].Dot((Vector&)vMatrix[j]) + vMatrix[j][3];

	Vector flb, brt, blb, frt, frb, brb, blt, flt;

	if (!gDrawManager.WorldToScreen(vTransformed[3], flb) ||
		!gDrawManager.WorldToScreen(vTransformed[0], blb) ||
		!gDrawManager.WorldToScreen(vTransformed[2], frb) ||
		!gDrawManager.WorldToScreen(vTransformed[6], blt) ||
		!gDrawManager.WorldToScreen(vTransformed[5], brt) ||
		!gDrawManager.WorldToScreen(vTransformed[4], frt) ||
		!gDrawManager.WorldToScreen(vTransformed[1], brb) ||
		!gDrawManager.WorldToScreen(vTransformed[7], flt))
		return; 

	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 0; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (top < arr[i].y)
			top = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (bottom > arr[i].y)
			bottom = arr[i].y;
	}

	float x = left;
	float y = bottom;
	float w = right - left;
	float h = top - bottom;

	x += ((right - left) / 8); //pseudo fix for those THICC boxes
	w -= ((right - left) / 8) * 2;

	int clrPlayerCol = GetPlayerColor(pEntity);
	//Color clrBoneCol = gCheatMenu.esp_bones == 1 ? Color::White() : g.esp_bones == 2 ? Color::Green() : clrPlayerCol;
	int iY = 0;
	//iHp is only for health bar
	int iHp = pEntity->GetHealth(), iMaxHp = pEntity->GetMaxHealth();
	if (iHp > iMaxHp)
		iHp = iMaxHp;
	

	if (gCheatMenu.building_esp)
	{
		//ESPBuildings(pEntity);
		// doesnt work
	}

	if (gCheatMenu.esp_box)
	{
		gDrawManager.OutlineRect(x - 1, y - 1, w + 2, h + 2, COLORCODE(0,0,0,255));
		gDrawManager.OutlineRect(x, y, w, h, clrPlayerCol);
		gDrawManager.OutlineRect(x + 1, y + 1, w - 2, h - 2, COLORCODE(0,0,0,255));
	}

	if (gCheatMenu.esp_health == 2 || gCheatMenu.esp_health == 3)
	{
		gDrawManager.OutlineRect(x - 6, y - 1, 5, h, COLORCODE(0,0,0,255));
		gDrawManager.DrawRect(x - 5, y + (h - (h / iMaxHp * iHp)) - 1, 3, h / iMaxHp * iHp, COLORCODE(0, 255, 0, 255));
	}

	if (gCheatMenu.esp_name)
	{
    	// Draw the player's name above the box, aligned to above
    	gDrawManager.DrawString((x + (w / 8)), y - 13, clrPlayerCol, pInfo.name);
    	//iY += gDrawManager.GetESPHeight();
	}
	if (gCheatMenu.esp_class)
	{
		gDrawManager.DrawString(x + w + 2, y + iY, clrPlayerCol, "%s", pEntity->szGetClass());
		iY += gDrawManager.GetESPHeight();
	}

	Vector b = pLocalEntity->GetAbsAngles();

	if (gCheatMenu.radarenable)
		gRadar.DrawRadarPoint(pEntity->GetAbsOrigin(), pLocalEntity->GetAbsOrigin(), QAngle(b.x, b.y, b.z), pEntity, clrPlayerCol);

	if (gCheatMenu.esp_health == 1 || gCheatMenu.esp_health == 3)
	{
		gDrawManager.DrawString(x + w + 2, y + iY, COLORCODE(0, 255, 0, 255), "%d HP", pEntity->GetHealth());
		iY += gDrawManager.GetESPHeight();
	}
	if (gCheatMenu.esp_cond)
	{
		if (pEntity->GetCond() & TFCond_Cloaked)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*CLOAKED*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_Bonked)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*BONKED*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_Disguised)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*DISGUISED*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_Healing)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*HEALING*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_Ubercharged)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*UBERED*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_UberchargeFading)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*UBER FADING*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_MarkedForDeath)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*MARKED FOR DEATH*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_Jarated)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*EW PISS");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_CritCola)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*CRIT COLA*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_MiniCrits)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*MINI CRITS*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_Taunting)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*TAUNTING*");
			iY += gDrawManager.GetESPHeight();
		}
		if (pEntity->GetCond() & TFCond_Zoomed)
		{
			gDrawManager.DrawString(x + w + 2, y + iY, CONDCOLOR, "*SCOPED*");
			iY += gDrawManager.GetESPHeight();
		}
	}
	if (!gCheatMenu.PlayerMode[pEntity->GetIndex()])
	{
		gDrawManager.DrawString(x + w + 2, y + iY, COLORCODE(15,150,150,255), "*Buddie :3/Ignored*");
		iY += gDrawManager.GetESPHeight();
	}
	if (gCheatMenu.esp_bones) //bones
	{
		int clrBoneCol = COLORCODE(255,255,255,255);
		static int iLeftArmBones[] = { 8, 7, 6, 4 };
		static int iRightArmBones[] = { 11, 10, 9, 4 };
		static int iHeadBones[] = { 0, 4, 1 };
		static int iLeftLegBones[] = { 14, 13, 1 };
		static int iRightLegBones[] = { 17, 16, 1 };

		DrawBone(pEntity, iLeftArmBones, 4, clrBoneCol);
		DrawBone(pEntity, iRightArmBones, 4, clrBoneCol);

		DrawBone(pEntity, iHeadBones, 3, clrBoneCol);

		DrawBone(pEntity, iLeftLegBones, 3, clrBoneCol);
		DrawBone(pEntity, iRightLegBones, 3, clrBoneCol);
	}

}

void ESP::DrawBone(CBaseEntity* pEntity, int* iBones, int count, int clrCol)
{
	for (int i = 0; i < count; i++)
	{
		if (i == count - 1)
			continue;

		Vector vBone1 = pEntity->get_hitbox_pos(iBones[i]);
		Vector vBone2 = pEntity->get_hitbox_pos(iBones[i + 1]);

		Vector vScr1, vScr2;

		if (!gDrawManager.WorldToScreen(vBone1, vScr1) || !gDrawManager.WorldToScreen(vBone2, vScr2))
			continue;

		gDrawManager.DrawLine(vScr1.x, vScr1.y, vScr2.x, vScr2.y, clrCol);
	}
}

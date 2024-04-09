#include "CMenu.h"
#include "SDK.h"
#include "CDrawManager.h"
#include "Colors.h"
#include "ConfigManager.h"
#include "Input.h"
#include <cstring>
CCheatMenu gCheatMenu;
int ScreenH;
int ScreenW;

char* szKeyNames[] = {
	"None", "Mouse 1", "Mouse 2", "Mouse 3", "Mouse 4", "Mouse 5", "Shift", "Alt", "F",
};

char* szHealthModes[] = {
	"OFF", "Text", "Bar", "Both",
};

char* szBoneModes[] = {
	"OFF", "White", "Health", "Team",
};
char* hvhpitchchar[] = {
	"Fake UP", "UP", "Fake Down", "Down",
};
char* hvhyawchar[] = {
	"Right", "Left", "Back", "Emotion", "Random", "Fake Sideways Right", "Fake Sideways Left", "Fake Right",
	"Half Backwards Left", "Half Backwards Right", "Fake Left"
};
/*
enum ESettingsHitboxes {
    SETT_HITBOX_HEAD  = 0,
    SETT_HITBOX_TORSO = 1,
    SETT_HITBOX_ARMS  = 2,
    SETT_HITBOX_LEGS  = 3,
};
*/
char* szHitboxes[] =
{
	"Auto", "Head", "Pelvis", "Lower Torse", "Lower Mid. Torse", "Upper Mid. Torse", "Upper Torse",
	"Left Upper Arm", "Left Lower Arm", "Left Hand", "Right Upper Arm",
	"Right Lower Arm", "Right Hand", "Left Hip", "Left Knee", "Left Foot",
	"Right Hip", "Right Knee", "Right Foot",
};


int CCheatMenu::AddItem(int nIndex, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isClassSwitch)
{
	strcpy(pMenu[nIndex].szTitle, szTitle);
	pMenu[nIndex].value = value;
	pMenu[nIndex].flMin = flMin;
	pMenu[nIndex].flMax = flMax;
	pMenu[nIndex].flStep = flStep;
	pMenu[nIndex].isClassSwitch = isClassSwitch;
	return (nIndex + 1);
}

void CCheatMenu::Render(void)
{
	gInts.Engine->GetScreenSize(ScreenW, ScreenH);

	int i = 0;

	i = AddItem(i, "Aimbot", &gCheatMenu.aimbot_switch, 0, 1, 1, true);
	if (gCheatMenu.aimbot_switch)
	{
		i = AddItem(i, " - Enabled", &gCheatMenu.aimbot_active, 0, 1, 1, false);
		i = AddItem(i, " - Silent", &gCheatMenu.aimbot_silent, 0, 1, 1, false);
		i = AddItem(i, " - FOV", &gCheatMenu.aimbot_fov, 0, 180, 1, false);
		//i = AddItem(i, " - Key", &gCheatMenu.aimbot_key, 0, 8, 1, false);
		i = AddItem(i, " - Hitscan", &gCheatMenu.aimbot_hitscan, 0, 1, 1, false);
		i = AddItem(i, " - Hitbox", &gCheatMenu.aimbot_hitbox, 0, 18, 1, false);
		i = AddItem(i, " - Autoshoot", &gCheatMenu.aimbot_autoshoot, 0, 1, 1, false);
		i = AddItem(i, " - AutoScope", &gCheatMenu.aimbot_autoscope, 0, 1, 1, false);
		i = AddItem(i, " - Infinite Distance", &gCheatMenu.aimbot_infinitedistance, 0, 1, 1, false);
		if (!gCheatMenu.aimbot_infinitedistance)
			i = AddItem(i, " - Distance", &gCheatMenu.aimbot_distance, 0, 100000, 1, false);
		i = AddItem(i, " - Zoomed Only", &gCheatMenu.aimbot_zoomedonly, 0, 1, 1, false);
	}

	i = AddItem(i, "Triggerbot", &gCheatMenu.triggerbot_switch, 0, 1, 1, true);
	if (gCheatMenu.triggerbot_switch)
	{
		i = AddItem(i, " - Enabled", &gCheatMenu.triggerbot_active, 0, 1, 1, false);
		//i = AddItem(i, " - Key", &gCheatMenu.triggerbot_key, 0, 8, 1, false);
		i = AddItem(i, " - Head Only", &gCheatMenu.triggerbot_headonly, 0, 1, 1, false);
	}

	i = AddItem(i, "Player List", &gCheatMenu.playerlist_switch, 0, 1, 1, true);
	for (int p = 1; p <= gInts.Engine->GetMaxClients(); p++)
	{
		if (p == me)
			continue;

		CBaseEntity *pPlayer = GetBaseEntity(p);

		if (pPlayer == NULL)
			continue;

		player_info_t pInfo;
		gInts.Engine->GetPlayerInfo(p, &pInfo);

		char szString[256];
		sprintf(szString, " - %s", pInfo.name);

		if (gCheatMenu.playerlist_switch)
			i = AddItem(i, szString, &gCheatMenu.PlayerMode[pPlayer->GetIndex()], 0, 2, 1, false);
	}


	i = AddItem(i, "ESP", &gCheatMenu.esp_switch, 0, 1, 1, true);
	if (gCheatMenu.esp_switch)
	{
		i = AddItem(i, " - Enabled", &gCheatMenu.esp_active, 0, 1, 1, false);
		i = AddItem(i, " - Enemies Only", &gCheatMenu.esp_enemyonly, 0, 1, 1, false);
		i = AddItem(i, " - Box", &gCheatMenu.esp_box, 0, 1, 1, false);
		i = AddItem(i, " - Name", &gCheatMenu.esp_name, 0, 1, 1, false);
		i = AddItem(i, " - Class", &gCheatMenu.esp_class, 0, 1, 1, false);
		i = AddItem(i, " - Health", &gCheatMenu.esp_health, 0, 3, 1, false);
		i = AddItem(i, " - Conditions", &gCheatMenu.esp_cond, 0, 1, 1, false);
		i = AddItem(i, " - pLocal ESP", &gCheatMenu.esp_local, 0, 1, 1, false);
		//i = AddItem(i, " - Building ESP", &gCheatMenu.building_esp, 0,1,1, false);,
		i = AddItem(i, " - Bones", &gCheatMenu.esp_bones, 0, 3, 1, false);
		//i = AddItem(i, " - Bones", &gCvars.esp_bones, 0, 3, 1, false);
	}
	i = AddItem(i , "Priorites", &gCheatMenu.priorties_switch, 0,1,1, true);

	if (gCheatMenu.priorties_switch)
	{
		i = AddItem(i, " - Ignore Cloaked", &gCheatMenu.priorties_ignorecloaked, 0,1,1, false);
		i = AddItem(i, " - Ignore Taunting", &gCheatMenu.priorties_ignoretaunting, 0,1,1, false);
		i = AddItem(i, " - Ignore Disguised", &gCheatMenu.priorties_ignoredisguised, 0,1,1, false);
		i = AddItem(i, " - Prefer Low HP", &gCheatMenu.aimbot_preferlowhp, 0, 1, 1, false);
		i = AddItem(i, " - Prefer Heavy", &gCheatMenu.priorties_preferheavy, 0, 1, 1, false);
		i = AddItem(i, " - Prefer Medick", &gCheatMenu.priorties_prefermedic, 0, 1, 1, false);
	}

	i = AddItem(i, "Radar", &gCheatMenu.radar_switch, 0, 1, 1, true);
	if (gCheatMenu.radar_switch)
	{
		i = AddItem(i, " - Enable Radar", &gCheatMenu.radarenable, 0,1,1, false);
		i = AddItem(i, " - Radar Postion X", &gCheatMenu.radar_posx, 0, ScreenW, 25, false);
		i = AddItem(i, " - Radar Postion Y", &gCheatMenu.radar_posy, 0, ScreenH, 25, false);
		i = AddItem(i, " - Radar Width", &gCheatMenu.radar_width, 0, 300, 25, false);
	}

	i = AddItem(i, "Settings & Configs", &gCheatMenu.settings_switch, 0, 1, 1, true);
	if (gCheatMenu.settings_switch)
	{
		i = AddItem(i, " - Menu Postion X", &gCheatMenu.iMenu_Pos_X, 0, ScreenW, 25, false);
		i = AddItem(i, " - Menu Postion Y", &gCheatMenu.iMenu_Pos_Y, 0, ScreenH, 25, false);
		i = AddItem(i, " - InfoBox Position X", &gCheatMenu.iInfo_Pos_X, 0, ScreenW, 25, false);
		i = AddItem(i, " - InfoBox Position Y", &gCheatMenu.iInfo_Pos_Y, 0, ScreenH, 25, false);
		i = AddItem(i, " - Save Config", &gCheatMenu.misc_saveconfig, 0,1,1, false);
		i = AddItem(i, " - Load Config", &gCheatMenu.misc_loadconfig, 0,1,1, false);
		if (gCheatMenu.misc_saveconfig)
		{
			gConfig.SaveConfig();
			gCheatMenu.misc_saveconfig = 0.0f; 
		}
		if (gCheatMenu.misc_loadconfig)
		{
			gConfig.LoadConfig();
			gCheatMenu.misc_loadconfig = 0.0f; 
		}
	}
	i = AddItem(i, "DT & Warp", &gCheatMenu.dt_n_warp_switch, 0, 1, 1, true);
	if (gCheatMenu.dt_n_warp_switch)
	{
		i = AddItem(i, " - Enable DT/Warp", &gCheatMenu.warp_enable, 0, 1, 1, false);
		i = AddItem(i, " - Ticks", &gCheatMenu.warp_value, 0, 35, 1, false);
		i = AddItem(i, " - Anti-Warp", &gCheatMenu.anti_warp, 0, 1, 1, false);
	}
	i = AddItem(i, "HvH", &gCheatMenu.hvh_switch, 0, 1, 1, true);
	if (gCheatMenu.hvh_switch)
	{
		i = AddItem(i, " - Enable AA", &gCheatMenu.hvh_enable, 0, 1, 1, false);
		i = AddItem(i, " - Pitch", &gCheatMenu.hvh_pitch, 0, 4, 1, false);
		i = AddItem(i, " - Yaw", &gCheatMenu.hvh_yaw, 0, 11, 1, false);
		i = AddItem(i, " - Resolver", &gCheatMenu.hvh_resolver, 0, 1, 1, false);
	}
	i = AddItem(i, "Misc", &gCheatMenu.misc_switch, 0, 1, 1, true);
	if (gCheatMenu.misc_switch)
	{
		i = AddItem(i, " - SpeedCrouch", &gCheatMenu.misc_speedcrouch, 0, 1, 1, false);
		i = AddItem(i, " - AutoBackStab", &gCheatMenu.misc_autobackstab, 0, 1, 1, false);
		i = AddItem(i, " - TauntSpin", &gCheatMenu.misc_tauntspin, 0, 1, 1, false);
		i = AddItem(i, " - Dump Players", &gCheatMenu.misc_dumpplayers, 0, 1, 1, false);
		if (gCheatMenu.misc_tauntspin)
			i = AddItem(i, " - Spin Speed", &gCheatMenu.misc_spinspeed, 0, 180, 1, false);

		if (gCheatMenu.misc_dumpplayers)
		{
			gConfig.DumpPlayers();
			gCheatMenu.misc_dumpplayers = 0.0f; 
		}
		i = AddItem(i, " - ThirdPerson", &gCheatMenu.misc_thirdperson, 0, 1, 1, false);
		i = AddItem(i, " - VMFoV", &gCheatMenu.misc_enablevmfov, 0, 1, 1, false);
		if (gCheatMenu.misc_enablevmfov)
		{
			i = AddItem(i, " - VMFoV Val", &gCheatMenu.misc_vmfov, 0, 180, 1, false);
		}
		i = AddItem(i, " - Bunnyhop", &gCheatMenu.misc_bunnyhop, 0, 1, 1, false);
		i = AddItem(i, " - Autostrafe", &gCheatMenu.misc_autostrafe, 0, 1, 1, false);
		i = AddItem(i, " - Followbot", &gCheatMenu.followbot_enabled, 0, 1, 1, false);
		//i = AddItem(i, " - SV_CHEATS bypass", &gCheatMenu.misc_svcheats, 0, 1, 1, false);
	}

	iMenuItems = i;
}
const Uint8 *keystate = SDL_GetKeyboardState(NULL);
int cur_x, cur_y;
//const Uint8 *mouseState = SDL_GetMouseState(cur_x, cur_y);
void CCheatMenu::HandleControls(void)
{	
	
	//

	
    // added sleep due to the key pressing being "too fast" 
	float flCurTime = gInts.Engine->Time();
	static float flNextSend = 0.0f;
	static float thesleeptime = 0.1f;
    //if (eventcode == 1)
	//{
		
	if (keystate[SDL_SCANCODE_INSERT] && flCurTime > flNextSend) //insert
	{
		flNextSend = (flCurTime + thesleeptime);
		gCheatMenu.bMenuActive = !gCheatMenu.bMenuActive;
    }

	if (gCheatMenu.bMenuActive)
	{
		
		if (keystate[SDL_SCANCODE_UP] && flCurTime > flNextSend) // Up
		{
			flNextSend = (flCurTime + thesleeptime);
			if (gCheatMenu.iMenuIndex > 0) gCheatMenu.iMenuIndex--;
			else gCheatMenu.iMenuIndex = gCheatMenu.iMenuItems - 1;
			//return 0;
		}
		if (keystate[SDL_SCANCODE_DOWN] && flCurTime > flNextSend) // Down
		{
			flNextSend = (flCurTime + thesleeptime);
			if (gCheatMenu.iMenuIndex < gCheatMenu.iMenuItems - 1) gCheatMenu.iMenuIndex++;
			else gCheatMenu.iMenuIndex = 0;
			//return 0;
		}
		if (keystate[SDL_SCANCODE_LEFT] && flCurTime > flNextSend) // Left
		{
			if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value && flCurTime > flNextSend)
			{
				flNextSend = (flCurTime + thesleeptime);
				gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] -= gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
				if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] < gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin)
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax;
			}
		}
		if (keystate[SDL_SCANCODE_RIGHT] && flCurTime > flNextSend) // Right
		{
			if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value && flCurTime > flNextSend)
			{
				flNextSend = (flCurTime + thesleeptime);
				gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] += gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flStep;
				if (gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] > gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMax)
					gCheatMenu.pMenu[gCheatMenu.iMenuIndex].value[0] = gCheatMenu.pMenu[gCheatMenu.iMenuIndex].flMin;
			}
		}

	}
	//}
}
/*
* Inspration From Old Nullcore (Circa 2018)
*/
void CCheatMenu::DrawInfo(int speedValue, float ticks)
{
	int x = iInfo_Pos_X,
    xx = x + 150,
    y = iInfo_Pos_Y,
    w = 200,
    h = 14;

	//int iMenuIndexNCC = 4;

	int clrColor = Color::COLORNULLCORE;

	//gDrawManager.DrawRect(x, y - (h + 4), w, 2 * h + 21, COLORCODE(20, 20, 20, 128));
	gDrawManager.DrawRect(x, y - (h + 4), w, 4 * h + 21, COLORCODE(20, 20, 20, 128));
	gDrawManager.OutlineRect(x, y - (h + 4), w, (h + 4), clrColor);

	// Draw the first text inside the box
	gDrawManager.DrawString(x + 4, y - (h + 4) + 30, clrColor, "Speed: %d", speedValue);

	// Draw the second text inside the box
	const char* thirdPersonStatus = gCheatMenu.isThirdPersonEnabled ? "Enabled" : "Disabled";
	const char* AntiAimStatus = gCheatMenu.hvh_enable ? "Enabled" : "Disabled";
	gDrawManager.DrawString(x + 4, y - (h + 4) + 20, clrColor, "Thirdperson: %s", thirdPersonStatus);

	gDrawManager.DrawString(x + 4, y - (h + 4) + 40, clrColor, "Anti-Aim: %s", AntiAimStatus);

	//gDrawManager.DrawString(x + 4, y - (h + 4) + 50, clrColor, "Uhh: %d", pEntity->GetVelocity().x);
	if (gCheatMenu.warp_enable)
		gDrawManager.DrawString(x + 4, y - (h + 4) + 50, clrColor, "Charged Ticks: %f", ticks);
	gDrawManager.OutlineRect(x - 1, y - (h + 4) - 1, w + 2, (h + 4), COLORCODE(0, 0, 0, 255)); // test
	gDrawManager.OutlineRect(x + 1, y - (h + 4) + 1, w - 2, (h + 4), COLORCODE(0, 0, 0, 255)); // test

	//gDrawManager.OutlineRect(x, y - (h + 4), w, 2 * h + 21, clrColor);
	gDrawManager.OutlineRect(x, y - (h + 4), w, 4 * h + 21, clrColor);
	//gDrawManager.OutlineRect(x - 1, (y - (h + 4)) - 1, w + 2, (2 * h + 21) + 2, COLORCODE(0, 0, 0, 255));
	//gDrawManager.OutlineRect(x + 1, (y - (h + 4)) + 1, w - 2, (2 * h + 21) - 2, COLORCODE(0, 0, 0, 255));

	gDrawManager.OutlineRect(x - 1, (y - (h + 4)) - 1, w + 2, (4 * h + 21) + 2, COLORCODE(0, 0, 0, 255));
	gDrawManager.OutlineRect(x + 1, (y - (h + 4)) + 1, w - 2, (4 * h + 21) - 2, COLORCODE(0, 0, 0, 255));


	// Draw the final text outside the box
	gDrawManager.DrawString(x + 4, y - 16, clrColor, "Information box");
}

void CCheatMenu::DrawMenu(void)
{
	int x = iMenu_Pos_X,
		xx = x + 150,
		y = iMenu_Pos_Y,
		w = 200,
		h = 14;
	
	gInts.Surface->GetCursorPosition(cur_x, cur_y);

	CBaseEntity *pLocal = GetBaseEntity(me);

	int clrColor = Color::COLORNULLCORE;

	gDrawManager.DrawRect(x, y - (h + 4), w, iMenuItems * h + 21, COLORCODE(20, 20, 20, 128));
	gDrawManager.OutlineRect(x, y - (h + 4), w, (h + 4), clrColor);

	//gDrawManager.DrawRect(x + 2, y - (h + 4), w - 4, (h + 4), clrColor);
	gDrawManager.OutlineRect(x - 1, y - (h + 4) - 1, w + 2, (h + 4), COLORCODE(0, 0, 0, 255)); // test
	gDrawManager.OutlineRect(x + 1, y - (h + 4) + 1, w - 2, (h + 4), COLORCODE(0, 0, 0, 255)); // test

	gDrawManager.OutlineRect(x, y - (h + 4), w, iMenuItems * h + 21, clrColor);

	gDrawManager.OutlineRect(x - 1, (y - (h + 4)) - 1, w + 2, (iMenuItems * h + 21) + 2, COLORCODE(0, 0, 0, 255));
	gDrawManager.OutlineRect(x + 1, (y - (h + 4)) + 1, w - 2, (iMenuItems * h + 21) - 2, COLORCODE(0, 0, 0, 255));

	gDrawManager.DrawString(x + 4, y - 16, COLORCODE(255,255,255,255), "TuxOwner");

	for (int i = 0; i < iMenuItems; i++)
	{
		if (i != iMenuIndex)
		{
			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), COLORCODE(255,255,255,255), "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i),  COLORCODE(255,255,255,255), "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), Color::COLORWHITE, pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::COLORWHITE : COLORCODE(105, 105, 105, 255), "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				if (!strcmp(pMenu[i].szTitle, " - Pitch"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::COLORWHITE : COLORCODE(105, 105, 105, 255), "%s", hvhpitchchar[(int)pMenu[i].value[0]]);
				}

				if (!strcmp(pMenu[i].szTitle, " - Yaw"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::COLORWHITE : COLORCODE(105, 105, 105, 255), "%s", hvhyawchar[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::COLORWHITE : COLORCODE(105, 105, 105, 255), "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::COLORWHITE, "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::COLORWHITE, "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), Color::COLORWHITE, !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] ? Color::COLORWHITE : COLORCODE(105, 105, 105, 255), pMenu[i].value[0] ? "ON" : "OFF");
				}

				else if (pMenu[i].value[0] >= 1 && pMenu[i].flMax > 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), pMenu[i].value[0] >= 1 ? Color::COLORWHITE : COLORCODE(105, 105, 105, 255), "%0.0f", pMenu[i].value[0]);
				}
			}
		}
		else
		{
			gDrawManager.DrawRect(x + 1, y + (h * i), w - 2, h, COLORCODE(255, 255, 255, 80));

			if (pMenu[i].isClassSwitch)
			{
				if (!(pMenu[i].value[0]))
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[+] %s", pMenu[i].szTitle);
				}
				else if (pMenu[i].value[0])
				{
					gDrawManager.DrawString(x + 2, y + (h * i), clrColor, "[-] %s", pMenu[i].szTitle);
				}
			}
			else
			{
				gDrawManager.DrawString(x + 4, y + (h * i), clrColor, pMenu[i].szTitle);

				if (!strcmp(pMenu[i].szTitle, " - Health"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHealthModes[(int)pMenu[i].value[0]]);
				}

				if (!strcmp(pMenu[i].szTitle, " - Pitch"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", hvhpitchchar[(int)pMenu[i].value[0]]);
				}
				
				if (!strcmp(pMenu[i].szTitle, " - Yaw"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", hvhyawchar[(int)pMenu[i].value[0]]);
				}

				else if (!strcmp(pMenu[i].szTitle, " - Bones"))
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szBoneModes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 18)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szHitboxes[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 8)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%s", szKeyNames[(int)pMenu[i].value[0]]);
				}

				else if (pMenu[i].flMax == 2)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, !pMenu[i].value[0] ? "Ignore" : pMenu[i].value[0] == 1 ? "Normal" : "Rage");
				}

				else if (pMenu[i].flMax == 1)
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, pMenu[i].value[0] ? "ON" : "OFF");
				}

				else
				{
					gDrawManager.DrawString(xx, y + (h * i), clrColor, "%0.0f", pMenu[i].value[0]);
				}
			}
		}
	}
}

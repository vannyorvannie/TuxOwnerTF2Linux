#pragma once
#include "SDK.h"
// pasted from potassium tf2 
// credits: gir and kalvin for this and source file

struct CMenuItems_t
{
	char szTitle[30];
	float* value;
	float flMin;
	float flMax;
	float flStep;
	bool isClassSwitch;
};

class CCheatMenu
{
public:
	void Render(void);
	void DrawMenu(void);
	int	 AddItem(int nIndex, char szTitle[30], float* value, float flMin, float flMax, float flStep, bool isClassHead);
    void HandleControls();
	void DrawInfo(int speedValue, float ticks);
	//private:
	bool bMenuActive;
	int iMenuIndex;

	int iMenuItems;
	CMenuItems_t pMenu[120];
	int iAimbotIndex = -1;
	
	float settings_switch;
	float iMenu_Pos_X = 300;
	float iMenu_Pos_Y = 75;
	float iInfo_Pos_X = 1150;
	float iInfo_Pos_Y = 600;
	float szsubstest;

	float aimbot_switch;
	float aimbot_silent;
	float aimbot_active;
	//float aimbot_key;
	float aimbot_hitscan;
	float aimbot_hitbox;
	float aimbot_autoshoot;
	float aimbot_zoomedonly;
	float aimbot_autoscope;
	float aimbot_fov;
	float aimbot_preferlowhp;
	float aimbot_infinitedistance;
	float aimbot_distance;

	float triggerbot_switch;
	float triggerbot_active;
	float triggerbot_key;
	float triggerbot_headonly;

	float esp_switch;
	float esp_active;
	float esp_enemyonly;
	float esp_box;
	float esp_name;
	float esp_cond;
	float esp_class;
	float esp_health;
	float esp_bones;
	float esp_local;
	float building_esp;
	

	float misc_speedcrouch;
	float misc_thirdperson;
	float misc_enablevmfov;
	float misc_vmfov;
	float misc_switch;
	float misc_bunnyhop;
	float misc_autostrafe;
	float misc_svcheats;
	float misc_tauntspin;
	float misc_spinspeed;
	float misc_autobackstab;
	float misc_dumpplayers;
	// i'll do kill say later

	float misc_saveconfig;
	float misc_loadconfig;
	/* hvh stuff */
	float hvh_switch;
	float hvh_enable;
	float hvh_yaw;
	float hvh_pitch;
	float hvh_resolver;

	float followbot_enabled;

	//float autobackstab;
	//float autobackstab_silent;
	bool isThirdPersonEnabled = false;
	
	/* DT */
	float dt_n_warp_switch;
	float warp_enable;
	float warp_value; 
	float WarpCharge = 0; // for the charge
	float anti_warp;
	bool IsDTing;
	bool IsRecharging;

	float playerlist_switch;
	float PlayerMode[64] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	float priorties_switch;
	float priorties_ignorecloaked;
	float priorties_ignoretaunting;
	float priorties_ignoredisguised;
	float priorties_ignorefriends;
	float priorties_preferheavy;
	float priorties_prefermedic;

	/* Radar */
	float radar_switch;
	float radartype;
	float radar_width;
	float radar_posx;
	float radar_posy;
	float radarenable;
};

extern CCheatMenu gCheatMenu;

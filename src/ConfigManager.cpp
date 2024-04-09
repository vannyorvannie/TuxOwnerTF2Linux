#include "ConfigManager.h"
#include <fstream>
#include <string>

CConfigManager gConfig;
/*
*   Made By Vannie 
*     16.01.2024
*/

void CConfigManager::SaveConfig() 
{
	std::ofstream output("TuxOwnerConfig.json");

	if (output.is_open()) 
    {
		Json::Value Save;
        /*
        *               Aimbot
        */
		Save["gAim"]["active"] = gCheatMenu.aimbot_active;
        Save["gAim"]["silent"] = gCheatMenu.aimbot_silent;
        Save["gAim"]["hitscan"] = gCheatMenu.aimbot_hitscan;
        Save["gAim"]["hitbox"] = gCheatMenu.aimbot_hitbox;
        Save["gAim"]["autoshoot"] = gCheatMenu.aimbot_autoshoot;
        Save["gAim"]["zoomedonly"] = gCheatMenu.aimbot_zoomedonly;
        Save["gAim"]["autoscope"] = gCheatMenu.aimbot_autoscope;
        Save["gAim"]["fov"] = gCheatMenu.aimbot_fov;
        /*
        *               Triggerbot
        */
        Save["gTrigger"]["key"] = gCheatMenu.triggerbot_key; // even tho i dont use this im just gonna save it just in case lol
        Save["gTrigger"]["active"] = gCheatMenu.triggerbot_active;
        Save["gTrigger"]["headonly"] = gCheatMenu.triggerbot_headonly;
        /*
        *                   ESP
        */
        Save["gESP"]["active"] = gCheatMenu.esp_active;
        Save["gESP"]["enemyonly"] = gCheatMenu.esp_enemyonly;
        Save["gESP"]["box"] = gCheatMenu.esp_box;
        Save["gESP"]["name"] = gCheatMenu.esp_name;
        Save["gESP"]["cond"] = gCheatMenu.esp_cond;
        Save["gESP"]["class"] = gCheatMenu.esp_class;
        Save["gESP"]["health"] = gCheatMenu.esp_health;
        Save["gESP"]["boner"] = gCheatMenu.esp_bones;
        Save["gESP"]["buildingesp"] = gCheatMenu.building_esp; // not used yet.
        /*
        *                   Misc
        */
        Save["gMisc"]["speedcrouch"] = gCheatMenu.misc_speedcrouch;
        Save["gMisc"]["thirdperson"] = gCheatMenu.misc_thirdperson;
        Save["gMisc"]["enablevmfov"] = gCheatMenu.misc_enablevmfov;
        Save["gMisc"]["vmfov"] = gCheatMenu.misc_vmfov;
        Save["gMisc"]["bhop"] = gCheatMenu.misc_bunnyhop;
        Save["gMisc"]["autostrafe"] = gCheatMenu.misc_autostrafe;
        //Save["gMisc"]["autokilltaunt"] = gCheatMenu.misc_autokilltaunt; // not used yet. (again)
        Save["gMisc"]["svcheats"] = gCheatMenu.misc_svcheats; // not used yet when i accomplish to fix ConVars. 
		/*
        *           DONE! Lets output it to the file!
        */
        output << Save;
		output.close();
	}
}

void CConfigManager::LoadConfig()
{
    std::ifstream load("TuxOwnerConfig.json");

	if (load.is_open()) {
		Json::Value Load;
		load >> Load;
        /*
        *               Aimbot
        */
		gCheatMenu.aimbot_active = Load["gAim"]["active"].asFloat();
        gCheatMenu.aimbot_autoscope = Load["gAim"]["autoscope"].asFloat();
        gCheatMenu.aimbot_hitbox = Load["gAim"]["hitbox"].asFloat();
        gCheatMenu.aimbot_fov = Load["gAim"]["fov"].asFloat();
        gCheatMenu.aimbot_autoshoot = Load["gAim"]["autoshoot"].asFloat();
        gCheatMenu.aimbot_hitscan = Load["gAim"]["hitscan"].asFloat();
        gCheatMenu.aimbot_zoomedonly = Load["gAim"]["zoomedonly"].asFloat();
        gCheatMenu.aimbot_silent = Load["gAim"]["silent"].asFloat();
        /*
        *               Triggerbot
        */
        gCheatMenu.triggerbot_active = Load["gTrigger"]["active"].asFloat();
        gCheatMenu.triggerbot_key = Load["gTrigger"]["key"].asFloat();
        gCheatMenu.triggerbot_headonly = Load["gTrigger"]["headonly"].asFloat();
        /*
        *                   ESP
        */
        gCheatMenu.esp_active = Load["gESP"]["active"].asFloat();
        gCheatMenu.esp_box = Load["gESP"]["box"].asFloat();
        gCheatMenu.esp_class = Load["gESP"]["class"].asFloat();
        gCheatMenu.esp_cond = Load["gESP"]["cond"].asFloat();
        gCheatMenu.esp_health = Load["gESP"]["health"].asFloat();
        gCheatMenu.esp_name = Load["gESP"]["name"].asFloat();
        gCheatMenu.esp_enemyonly = Load["gESP"]["active"].asFloat();
        gCheatMenu.esp_bones = Load["gESP"]["boner"].asFloat();
        gCheatMenu.building_esp = Load["gESP"]["buildingesp"].asFloat();
        /*
        *                   Misc
        */
        gCheatMenu.misc_vmfov = Load["gMisc"]["vmfov"].asFloat();
        gCheatMenu.misc_thirdperson = Load["gMisc"]["thirdperson"].asFloat();
        gCheatMenu.misc_enablevmfov = Load["gMisc"]["enablevmfov"].asFloat();
        gCheatMenu.misc_svcheats = Load["gMisc"]["svcheats"].asFloat();
        gCheatMenu.misc_bunnyhop = Load["gMisc"]["bhop"].asFloat();
        gCheatMenu.misc_autostrafe = Load["gMisc"]["autostrafe"].asFloat();
        gCheatMenu.misc_speedcrouch = Load["gMisc"]["speedcrouch"].asFloat();
        //gCheatMenu.misc_autokilltaunt = Load["gMisc"]["autokilltaunt"].asFloat();

        load.close(); // done reading the file. Close it.
    }
    
}
/*
*   Not really supposed to be here but who cares?
*/
void CConfigManager::DumpPlayers()
{
    std::ofstream output("players.txt", std::ios_base::app); // app allows to push to file
    for (int i = 1; i <= gInts.Engine->GetMaxClients(); i++)
	{
		if (i == me)
			continue;
		CBaseEntity* pEntity = GetBaseEntity(i);

		if (!pEntity)
			continue;
		
        player_info_t pInfo;
        if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &pInfo))
            continue;
        
        output << std::to_string(pInfo.friendsID).c_str() << " : " << pInfo.name << std::endl;
	}
    output.close();
}
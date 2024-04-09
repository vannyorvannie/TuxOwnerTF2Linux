#include "SDK.h"
#include "json.h"
#include "CMenu.h"
class CConfigManager
{
public:
    void SaveConfig();
    void LoadConfig(); 
    void DumpPlayers();
};

extern CConfigManager gConfig;
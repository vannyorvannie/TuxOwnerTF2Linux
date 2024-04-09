#pragma once

#include "SDK.h"

class CGlobalzz {
public:
	//CBaseEntity* local{};
	//CUserCmd* cmd{};
	//CUserCmd original_cmd{};
	bool isfiring, silenttime;
	bool* sendpacket;
	//Vector realangle;
};

bool Hooked_CreateMove(void *ClientMode, float input_sample_frametime, CUserCmd* pCommand);

void FrameStageNotifyThink(void* CHLClient, void *_this, ClientFrameStage_t Stage);


extern CGlobalzz g;

#include "HvH.h"


/*
*   Pasted From NaCl.
*   Very Simple Anti-Aim System i guess
*   23-01-2023
*/



CHvH gHvH;

void movementfix(CUserCmd* pCmd, Vector vOldAngles, float fOldSidemove, float fOldForward)
{
	int e = rand() % 3;

	Vector curAngs = pCmd->viewangles;
	float fRot = 90;
	//Vector nwANG = Vector(-89, curAngs.y + fRot, 0);
	//pCmd->viewangles = nwANG;

	float deltaView = pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if (vOldAngles.y < 0.f)
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if (pCmd->viewangles.y < 0.0f)
		f2 = 360.0f + pCmd->viewangles.y;
	else
		f2 = pCmd->viewangles.y;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
}

float RandFloatRange(float min, float max)
{
	return (min + 1) + (((float)rand()) / (float)RAND_MAX) * (max - (min + 1));
}


void CHvH::Run(CBaseEntity* pLocal, CUserCmd* pCmd)
{
  	// anti-aim isn't enabled, don't bother.
  	if (!gCheatMenu.hvh_enable)
    	return;

  	// NOTE: Riley; Instead of checking for IN_ATTACK like this from CUserCmd...
  	// We should check to see if we are reloading, if our weapon is charged
  	// (in that case, beggars bazooka, huntsman, etc.), check if we are not
 	// reloading, and then check if our weapon is able to fire (time between gunfire, dragon's fury, etc.)
  	// This will then mean: Anti-Aim doesn't stick disabled while holding Mouse1
  	// You can also re-use this check for aimbot.
  	// useful reference material from ateris-v3_src
  	// https://github.com/eternal-blu/ateris-technologies/blob/main/ateris-internal/Framework/SDK/TF2/Entities.cpp#L372
  	// https://github.com/eternal-blu/ateris-technologies/blob/main/ateris-internal/Framework/SDK/TF2/Entities.cpp#L235
  	if (pCmd->buttons & IN_ATTACK)
    	return;
	if (pCmd->buttons & IN_RELOAD)
		return;
  	// anti-aim isn't enabled, don't bother.
  	if (!gCheatMenu.hvh_enable)
    	return;

  // NOTE: Riley; Instead of checking for IN_ATTACK like this from CUserCmd...
  // We should check to see if we are reloading, if our weapon is charged
  // (in that case, beggars bazooka, huntsman, etc.), check if we are not
  // reloading, and then check if our weapon is able to fire (time between gunfire, dragon's fury, etc.)
  // This will then mean: Anti-Aim doesn't stick disabled while holding Mouse1
  // You can also re-use this check for aimbot.
  // useful reference material from ateris-v3_src
  // https://github.com/eternal-blu/ateris-technologies/blob/main/ateris-internal/Framework/SDK/TF2/Entities.cpp#L372
  // https://github.com/eternal-blu/ateris-technologies/blob/main/ateris-internal/Framework/SDK/TF2/Entities.cpp#L235
  if (pCmd->buttons & IN_ATTACK)
    return;

	Vector vOldAngles = pCmd->viewangles;
	float fOldForward = pCmd->forwardmove;
	float fOldSidemove = pCmd->sidemove;
	Vector/*&*/ angles = pCmd->viewangles;
	static bool flip = false;
	bool clamp = false;

	// I hate this. But you cannot use float on switches, so we have to cast it. 
	int hvh_pitchintegerified = static_cast<int>(gCheatMenu.hvh_pitch);
	int hvh_yawintegerified   = static_cast<int>(gCheatMenu.hvh_yaw);
  	switch(hvh_pitchintegerified)
	{
  	case 0:
    	break;
  	case 1:
    	angles.x = -271.f;
    	break;
  	case 2:
    	angles.x = -89.f;
    	break;
  	case 3:
    	angles.y = 271.f;
    	break;
  	case 4:
    	angles.y = 89.f;
    	break;
  	}

  	switch(hvh_yawintegerified)
	{
  	case 0:
    	break;
  	case 1:
    	angles.y -= -90.0f; // NOTE: Riley; These probably need to be changed around but I cannot test.
    	break;
  	case 2:
    	angles.y += 90.0f;
    	break;
  	case 3:
    	angles.y -= 180;
    	break;
  	case 4:
    	angles.y = 89.99985719438652715f; 
    	break;
  	case 5:
    	angles.y = RandFloatRange(-180.0f, 180.0f); // TODO: remove this
    	break;
  	case 6:{
    	if (*g.sendpacket)
        	angles.y += 90.0f;
      	else
        	angles.y += -90.0f;
   	 	break;
  	}
  	case 7:{
    	if (*g.sendpacket)
        	angles.y += -90.0f;
      	else
        	angles.y += 90.0f;
    	break;
  	}
  	case 8:{
    	if (*g.sendpacket) 
      		angles.y += -90.0f;
    	else 
      		angles.y += 0.0f;
    	break;
  	}
  	case 9:{
    	if (*g.sendpacket)
        	angles.y += 135.0f;
      	else
        	angles.y += -135.0f;
    	break;
  	}
  	case 10:{
    	if (*g.sendpacket)
        	angles.y += -135.0f;
      	else
        	angles.y += 135.0f;
    	break;
  	}
  	case 11:{
    	if (*g.sendpacket)
        	angles.y += 90.0f;
      	else
        	angles.y += 0.0f;
    	break;
  	}
 	}

	pCmd->viewangles = angles;
	movementfix(pCmd, vOldAngles, fOldSidemove, fOldForward);
}

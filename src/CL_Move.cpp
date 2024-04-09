#include "CL_Move.h"
#include "CMenu.h"
#include <SDL2/SDL.h>


DetourHook CL_Move_Detour;



void Hooked_CL_Move(float accumulated_extra_samples, bool bFinalTick) 
{
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	auto Original = (CL_Move_t)CL_Move_Detour.GetOriginalFunc();
    if (gCheatMenu.warp_enable && keystate[SDL_SCANCODE_R]) // R
	{
		for (int i = 0; i < gCheatMenu.WarpCharge; i++)
		{
			gCheatMenu.IsDTing = true;
			Original(accumulated_extra_samples, bFinalTick);
			gCheatMenu.WarpCharge--;
		}
	}
	else gCheatMenu.IsDTing = false;

	if (keystate[SDL_SCANCODE_F] && gCheatMenu.WarpCharge < gCheatMenu.warp_value) // F
		gCheatMenu.WarpCharge++;
	else
		Original(accumulated_extra_samples, bFinalTick);

	CL_Move_Detour.RestorePatch();
}
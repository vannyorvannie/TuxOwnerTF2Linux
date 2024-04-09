#include "SDK.h"
#include "Detour.h"

void Hooked_CL_Move(float accumulated_extra_samples, bool bFinalTick);
typedef void(*CL_Move_t)(float accumulated_extra_samples, bool bFinalTick);
extern DetourHook CL_Move_Detour;

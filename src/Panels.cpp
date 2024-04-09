#include "SDK.h"
#include "Panels.h"
#include "CDrawManager.h"
#include "Features/ESP/ESP.h"
#include "CNetVars.h"
#include "CMenu.h"
#include "CSignature.h"
#include "Radar.h"
#include "Client.h"
//#include "Colors.h"
CScreenSize gScreenSize;
//===================================================================================
void Hooked_PaintTraverse( void *pPanels, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
{
	try
	{
		VMTManager& hook = VMTManager::GetHook(pPanels); //Get a pointer to the instance of your VMTManager with the function GetHook.
		hook.GetMethod<void(*)(void *, unsigned int, bool, bool)>(gOffsets.iPaintTraverseOffset)(pPanels, vguiPanel, forceRepaint, allowForce); //Call the original.

		static unsigned int vguiMatSystemTopPanel;
		static unsigned int vguiFocusOverlayPanel;
		/*
		if (vguiMatSystemTopPanel == 0)
		{
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'M' && szName[3] == 'S') //Look for MatSystemTopPanel without using slow operations like strcmp or strstr.
			{
				vguiMatSystemTopPanel = vguiPanel;
				Intro();
			}
		}
		*/
		if (vguiFocusOverlayPanel == NULL)
		{											//FocusOverlayPanel
			const char* szName = gInts.Panels->GetName(vguiPanel);
			if( szName[0] == 'F' && szName[5] == 'O' &&  szName[12] == 'P' )
			{
				vguiFocusOverlayPanel = vguiPanel;
				Intro();
			}
		}

		
		
		if ( vguiFocusOverlayPanel == vguiPanel ) //If we're on MatSystemTopPanel, call our drawing code.
		{
			//This section will be called when the player is not at the menu game and can see the screen or not taking a screenshot.
			//gDrawManager.DrawString( 200, 200, gDrawManager.dwGetTeamColor(3), "Welcome to Darkstorm"); //Remove this if you want.

			gCheatMenu.HandleControls(); // call the Handle Controls function to handle controls of the menu.. Duh!
			if (gCheatMenu.bMenuActive)
			{
				gCheatMenu.DrawMenu();
				gCheatMenu.Render();
			}

			bool check = false;
			if (!gInts.Engine->IsInGame() && check == false)
			{
				void** pure_addr = nullptr;
				if (!pure_addr)
				{
					pure_addr = *reinterpret_cast<void***>(gSignatures.GetEngineSignature("A1 F8 93 8C 00 85 C0") + 1);
				} *
					pure_addr = (void*)0;

				check = true;
			}


			CBaseEntity* pBaseLocalEnt = gInts.EntList->GetClientEntity(me);  //Grab the local player's entity.

			if (pBaseLocalEnt == NULL) //Always check for null pointers.
				return;



			/*		 |
			sv_pure? v
			A1 F8 93 8C 00 85 C0
			*/

			

			gRadar.DrawRadarBack();
			gESP.StartThisshit(pBaseLocalEnt);
			Vector velocity = pBaseLocalEnt->GetVelocityLocal();
			float g_speed = 0.f;
			g_speed = abs(sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]));

			int speedDigits = static_cast<int>(g_speed);
			while (speedDigits >= 1000) {
    			speedDigits /= 10;
			}
			//int asswipe = static_cast<int>(velocity.x) * 1000000 + static_cast<int>(velocity.y) * 1000 + static_cast<int>(velocity.z);
			//gDrawManager.DrawString(200,200, COLORCODE(255,255,255,255),  "Velocity: %d", pBaseLocalEnt->GetVelocityLocal());
			//int asswipe = static_cast<int>(velocity.x * 1000); // Assuming x is a float or double
			gCheatMenu.DrawInfo(speedDigits, gCheatMenu.WarpCharge); // 10 for testing, i'll add velocity later mate.
		}
	}
	catch(...)
	{
		//Log::Fatal("Failed PaintTraverse");
	}
}
//===================================================================================
void Intro( void )
{
	try
	{
		gDrawManager.Initialize(); //Initalize the drawing class.
		gNetVars.Initialize(); // if we dont do this, we will crash.
		Log::Msg("Injection Successful"); //If the module got here without crashing, it is good day.

		Log::Debug("Height=%d, Width=%d",gScreenSize.iScreenHeight, gScreenSize.iScreenWidth);
	}
	catch(...)
	{
		//Log::Fatal("Failed Intro");
	}
}

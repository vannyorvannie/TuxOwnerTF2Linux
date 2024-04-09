#include "Radar.h"

CRadar gRadar;

void CRadar::DrawRadarPoint(Vector vOriginX, Vector vOriginY, QAngle qAngle, CBaseEntity *pBaseEntity, int dwTeamColor)
{
	CBaseEntity* pLocalEntity = GetBaseEntity(me);

	if (pLocalEntity == NULL)
		return;

	float flDeltaX = vOriginX.x - vOriginY.x;
	float flDeltaY = vOriginX.y - vOriginY.y;

	float flAngle = qAngle.y;

	float flYaw = (flAngle)* (3.14159265358979323846 / 180.0);
	float flMainViewAngles_CosYaw = cos(flYaw);
	float flMainViewAngles_SinYaw = sin(flYaw);

	// rotate
	float x = flDeltaY * (-flMainViewAngles_CosYaw) + flDeltaX * flMainViewAngles_SinYaw;
	float y = flDeltaX * (-flMainViewAngles_CosYaw) - flDeltaY * flMainViewAngles_SinYaw;

	float flRange = 2000;

	if (fabs(x) > flRange || fabs(y) > flRange)
	{
		// clipping
		if (y > x)
		{
			if (y > -x)
			{
				x = flRange * x / y;
				y = flRange;
			}
			else
			{
				y = -flRange * y / x;
				x = -flRange;
			}
		}
		else
		{
			if (y > -x)
			{
				y = flRange * y / x;
				x = flRange;
			}
			else
			{
				x = -flRange * x / y;
				y = -flRange;
			}
		}
	}
	if (gCheatMenu.radarenable)
	{
		int	iScreenX = gCheatMenu.radar_posx + int(x / flRange * gCheatMenu.radar_width );
		int iScreenY = gCheatMenu.radar_posy + int(y / flRange * gCheatMenu.radar_width );

		gDrawManager.DrawRect(iScreenX - 3, iScreenY - 3, 7, 7, COLORCODE(0, 0, 0, 255));
		gDrawManager.DrawRect(iScreenX - 2, iScreenY - 2, 5, 5, dwTeamColor);
	}
}
//===================================================================================
void CRadar::DrawRadarBack()
{
	if (gCheatMenu.esp_active)
	{

		//if (dragg && !InputSys::Get().IsKeyDown(VK_LBUTTON))
		//	dragg = false;

		//int cur_x, cur_y;
		//gInts.Surface->GetCursorPosition(cur_x, cur_y);

		int t_x = gCheatMenu.radar_posx;
		int t_y = gCheatMenu.radar_posy;
		Vector2 tl(t_x + m_x, t_y + m_y);
		Vector2 br(tl.x + m_width, tl.y + m_height);


		int lmao1;
		int lmao2;
		//int iCenterX = m_flx;
		//int iCenterY = m_fly;
		int iCenterX = gCheatMenu.radar_posx;
		int iCenterY = gCheatMenu.radar_posy;
		int iSize = gCheatMenu.radar_width;


		int m_iScreenWidth = gScreenSize.iScreenWidth;
		int m_iScreenHeight = gScreenSize.iScreenHeight;

		int x = m_iScreenWidth / 2;
		int y = m_iScreenHeight / 2;
		if (gCheatMenu.radarenable)
		{
			gDrawManager.DrawRect(iCenterX - iSize, iCenterY - iSize, 2 * iSize + 2, 2 * iSize + 2, COLORCODE(30, 30, 30, 128));
			gDrawManager.DrawRect(iCenterX, iCenterY - iSize, 1, 2 * iSize, COLORCODE(0, 255, 0, 255));
			gDrawManager.DrawRect(iCenterX - iSize, iCenterY, 2 * iSize, 1, COLORCODE(0, 255, 0, 255));
			gDrawManager.DrawRectRadar(iCenterX - iSize, iCenterY - iSize, 2 * iSize + 2, 2 * iSize + 2, COLORCODE(0, 255, 0,255));
		}
	}
}



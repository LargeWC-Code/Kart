#include "kart.h"

void main()
{
	UCDevice3D* Device3D = UIGetDevice3D();

	//	ucINT ScreenCX = Device3D->GetScreenSize().cx;
	//	ucINT ScreenCY = Device3D->GetScreenSize().cy;
	GetScreenControl()->BackColor = 0xFF000000;

	ucINT Scale = 100;//50;
	Device3D->SetScreenScale(100 * Scale / 100);

	UCGame Game;
	Game.AutoSize = 1;
	Game.WinSize = UCSize(SCREEN_CX * Scale / 100, SCREEN_CY * Scale / 100);
	Game.Size = UCSize(SCREEN_CX, SCREEN_CY);
	Ckart* pkart = new Ckart;
	Game.Run(pkart);
	delete pkart;
}

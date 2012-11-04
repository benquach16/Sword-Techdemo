#include "stdafx.h"
#include "hud.h"

CHud::CHud(irr::IrrlichtDevice *graphics) : graphics(graphics)
{
	//create health bar
	t = graphics->getVideoDriver()->getScreenSize();

	health_text = graphics->getGUIEnvironment()->addStaticText(L"+100", rect<s32>(10,t.Height-40,300,t.Height));
	health_text->setOverrideColor(SColor(255,255,255,255));
}

void CHud::run(CPlayer *player)
{
	stringw h;
	h=L"+";
	h+=player->getHealth();
	health_text->setText(h.c_str());
}
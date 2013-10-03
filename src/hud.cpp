//////////////////////////////////////////////////////////////////////
///Copyright (C) 2012 Benjamin Quach
//
//This file is part of the sword tech demo program
//
//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "hud.h"

#define HEIGHT 20
#define WIDTH 100

CHud::CHud(irr::IrrlichtDevice *graphics) : graphics(graphics)
{
	//create health bar
	t = graphics->getVideoDriver()->getScreenSize();

}

void CHud::run(CPlayer *player)
{
	//get coords of target
	int health = player->getHealth();
	int stamina = player->getStamina();
	vector2d<s32> pos(0,t.Height-HEIGHT);
	graphics->getVideoDriver()->draw2DRectangle(rect<s32>(pos.X,pos.Y-10,pos.X+WIDTH,pos.Y+HEIGHT-20),
		SColor(255,0,0,0),SColor(255,0,0,0),SColor(255,0,0,0),SColor(255,0,0,0));
	graphics->getVideoDriver()->draw2DRectangle(rect<s32>(pos.X+1,pos.Y+1-10,pos.X-1+stamina,pos.Y+HEIGHT-1-10),
		SColor(255,255,255,0),SColor(255,255,255,0),SColor(255,255,255,0),SColor(255,255,255,0));
	graphics->getVideoDriver()->draw2DRectangle(rect<s32>(pos.X,pos.Y,pos.X+WIDTH,pos.Y+HEIGHT),
		SColor(255,0,0,0),SColor(255,0,0,0),SColor(255,0,0,0),SColor(255,0,0,0));
	graphics->getVideoDriver()->draw2DRectangle(rect<s32>(pos.X+1,pos.Y+1,pos.X-1+health,pos.Y+HEIGHT-1),
		SColor(255,0,255,0),SColor(255,0,255,0),SColor(255,0,255,0),SColor(255,0,255,0));


}
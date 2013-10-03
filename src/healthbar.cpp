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
#include "healthbar.h"

#define BAR_HEIGHT 80

CHealthBar::CHealthBar(irr::IrrlichtDevice *graphics, scene::ISceneNode *target) : graphics(graphics), target_(target)
{
	//constructor
	maxhealth = 100;
}

void CHealthBar::run(int health)
{
	//get coords of target
	vector3df tmp = target_->getPosition();
	tmp.Y += BAR_HEIGHT;
	vector2d<s32> pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(tmp,graphics->getSceneManager()->getActiveCamera());
	graphics->getVideoDriver()->draw2DRectangle(rect<s32>(pos.X-50,pos.Y,pos.X+50,pos.Y+10),SColor(255,0,0,0),SColor(255,0,0,0),SColor(255,0,0,0),SColor(255,0,0,0));
	graphics->getVideoDriver()->draw2DRectangle(rect<s32>(pos.X-49,pos.Y+1,pos.X-51+health,pos.Y+9),SColor(255,0,255,0),SColor(255,0,255,0),SColor(255,0,255,0),SColor(255,0,255,0));
}
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

#ifndef _HEALTHBAR_H_
#define _HEALTHBAR_H_

#include "irrlicht.h"

using namespace irr;
using namespace scene;
using namespace core;
using namespace gui;
using namespace video;

//displays a health bar above character
class CHealthBar
{
public:
	CHealthBar(irr::IrrlichtDevice *graphics, scene::ISceneNode *target);
	void run(int health);
	~CHealthBar();

private:
	int maxhealth;
	irr::IrrlichtDevice *graphics;
	scene::ISceneNode *target_;
};

#endif
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

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "irrlicht.h"
#include "Newton.h"
#include "person.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

//player class

class CPlayer : public CPerson
{
public:
	//constructor func
	CPlayer(irr::IrrlichtDevice *graphics = 0,
		NewtonWorld *world = 0,
		core::vector3df &pos = vector3df(0,0,0),
		core::vector3df &rot = vector3df(0,0,0),
		const char *name = "Player");
	~CPlayer();
	friend class CPerson;

	void run(f32 frameDeltaTime);
	void rotateFacingX(float x);
	void rotateFacingY(float y);

	void deathScreen();

	void setHeadRotation(vector3df rot);
	vector3df getHeadRotation();

	RakString getName()
	{
		return name_;
	}


private:
	irr::IrrlichtDevice *graphics;
	vector3df head_rotation;

	RakString name_;
	gui::IGUIStaticText *player_name;
	int swing_force_timer;
};

#endif
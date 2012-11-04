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

//runs through every object
//manages object code

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#pragma once

#include "irrlicht.h"
#include "irrklang.h"
#include "Newton.h"
#include "person.h"
#include "player.h"
#include "effect.h"
#include "spark.h"
#include "blood.h"
#include "vector"

using namespace irr;
using namespace core;
using namespace scene;



class CGameManager
{
public:
	CGameManager(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, NewtonWorld *world);
	void run(f32 frameDeltaTime);
	void runEffects();
	void addPerson(CPerson *person);
	void addEffect(CEffect *effect);
	bool checkCollision(SObject *a, SObject *b);
	vector3df checkCollisionPoint(SObject *a, SObject *b);
	~CGameManager();

private:
	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;
	NewtonWorld *world;

	std::vector<CPerson*> person_manager;
	std::vector<CEffect*> effect_manager;

};

#endif
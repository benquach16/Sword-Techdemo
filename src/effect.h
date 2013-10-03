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

//effects should be particle systems
//this is just a base class
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "irrlicht.h"
#include "irrklang.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

class CEffect
{
public:
	CEffect(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, vector3df &pos);
	virtual void run();
	virtual void drop();
	bool getEnd();
	~CEffect();

private:
	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;

	scene::IParticleEmitter *emitter;
	scene::IParticleSystemSceneNode *ps;
	vector3df pos;

	int end_time;
	bool end;
};

#endif
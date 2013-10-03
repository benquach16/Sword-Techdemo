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

#ifndef _SPARK_H_
#define _SPARK_H_

#include "effect.h"

class CSpark : public CEffect
{
public:
	CSpark(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, vector3df& pos);
	virtual void drop();
	~CSpark();

private:
	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;

	scene::IParticleEmitter *emitter;
	scene::IParticleSystemSceneNode *ps;


	int end_time;
	bool end;
};

#endif
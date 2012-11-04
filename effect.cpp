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
#include "effect.h"

CEffect::CEffect(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, vector3df &pos) : 
graphics(graphics), sound(sound), pos(pos), end(false)
{
	end_time = graphics->getTimer()->getTime()+500;
	//ps = graphics->getSceneManager()->addParticleSystemSceneNode(true);
	//emitter = ps->createPointEmitter(pos);
}

CEffect::~CEffect()
{
	//ps->remove();
}

void CEffect::run()
{
	if(end_time < graphics->getTimer()->getTime())
	{
		end=true;
	}
}

bool CEffect::getEnd()
{
	return end;
}

void CEffect::drop()
{
	delete this;
}
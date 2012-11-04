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
#include "blood.h"

CBlood::CBlood(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, vector3df &pos) : CEffect(graphics,sound,pos), graphics(graphics), sound(sound)
{
	end=false;
	end_time = graphics->getTimer()->getTime()+3000;
	ps = graphics->getSceneManager()->addParticleSystemSceneNode(true);
	ps->setPosition(pos);
	emitter = ps->createSphereEmitter(vector3df(0,0,0), 2,vector3df(0,-0.01,0), 20,30, SColor(255,255,255,255), SColor(255,255,255,255), 500,1000,0,dimension2df(10,10),dimension2df(20,20));
	ps->setEmitter(emitter);
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialTexture(0, graphics->getVideoDriver()->getTexture("res/textures/blood.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	scene::IParticleAffector *af = ps->createAttractionAffector(pos, -20);
	ps->addAffector(af);
}

CBlood::~CBlood()
{
	ps->remove();
}

void CBlood::drop()
{
	delete this;
}


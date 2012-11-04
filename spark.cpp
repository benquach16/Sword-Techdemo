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
#include "spark.h"

CSpark::CSpark(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, vector3df &pos) : CEffect(graphics,sound,pos)
{
	end=false;
	end_time = graphics->getTimer()->getTime()+100;
	ps = graphics->getSceneManager()->addParticleSystemSceneNode(true);
	ps->setPosition(pos);
	emitter = ps->createSphereEmitter(vector3df(0,0,0), 1,vector3df(0,0,0), 2,8, SColor(255,255,255,255), SColor(255,255,255,255), 100,200,0,dimension2df(1,1),dimension2df(1,1));
	ps->setEmitter(emitter);
	ps->setMaterialFlag(video::EMF_LIGHTING,false);
	ps->setMaterialTexture(0, graphics->getVideoDriver()->getTexture("res/textures/spark.png"));
	ps->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	scene::IParticleAffector *af = ps->createAttractionAffector(pos, -50);
	ps->addAffector(af);
}

CSpark::~CSpark()
{
	ps->remove();
}

void CSpark::drop()
{
	delete this;
}
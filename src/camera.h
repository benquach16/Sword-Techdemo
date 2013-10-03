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

//custom camera scenenode so i can have more control over the camera
//such as making it shake for special effects

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class CCamera 
{
public:
	CCamera(irr::IrrlichtDevice *graphics = 0, vector3df &pos = vector3df(0,0,0), vector3df &rot = vector3df(0,0,0));
	~CCamera();

private:
	irr::IrrlichtDevice *graphics;
	scene::ICameraSceneNode *camera;
};

#endif
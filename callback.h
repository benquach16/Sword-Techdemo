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


#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include "irrlicht.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace scene;
using namespace core;

//use this if i ever want to implement shaders

class CCallback : public IShaderConstantSetCallback
{
public:
	virtual void OnSetConstants(video::IMaterialRendererServices *services, s32 userData)
	{
		core::matrix4 projection = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
		core::matrix4 world = services->getVideoDriver()->getTransform(video::ETS_WORLD);
		core::matrix4 view = services->getVideoDriver()->getTransform(video::ETS_VIEW);
		
	}
private:

};

#endif
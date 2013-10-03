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

#ifndef _HUD_H_
#define _HUD_H_

#include "irrlicht.h"
#include "player.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace gui;

class CHud
{
public:
	CHud(irr::IrrlichtDevice *graphics);
	void run(CPlayer *player);
	~CHud();

private:
	irr::IrrlichtDevice *graphics;
	dimension2d<f32> t;

};

#endif
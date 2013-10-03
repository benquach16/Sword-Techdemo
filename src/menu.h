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

#ifndef _MENU_H_
#define _MENU_H_

#pragma once

#include "irrlicht.h"

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;
using namespace scene;

class CMenu
{
public:
	CMenu(irr::IrrlichtDevice *graphics);
	~CMenu();

	void run();
	void setVisible(bool visible)
	{
		window->setVisible(visible);
	}
	bool isVisible()
	{
		return window->isVisible();
	}
	core::dimension2d<f32> getScreenSize()
	{
		return screen_size;
	}
	rect<s32> getWindowSize()
	{
		return window_size;
	}
	gui::IGUIWindow *getWindow()
	{
		return window;
	}
	
private:

	irr::IrrlichtDevice *graphics;
	gui::IGUIWindow *window;

	core::dimension2d<f32> screen_size;
	rect<s32> window_size;
};

#endif
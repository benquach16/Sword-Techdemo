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
#include "menu.h"

CMenu::CMenu(irr::IrrlichtDevice *graphics) : graphics(graphics)
{
	screen_size = graphics->getVideoDriver()->getScreenSize();
	window_size = rect<s32>(screen_size.Width/2-300,screen_size.Height/2-300,screen_size.Width/2+300,screen_size.Height/2+300);
	window = graphics->getGUIEnvironment()->addWindow(window_size);
	window->setDrawTitlebar(false);
	window->getCloseButton()->setVisible(false);
	window->setDraggable(false);
}

CMenu::~CMenu()
{
}
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

#ifndef _OPTIONS_MENU_H_
#define _OPTIONS_MENU_H_

#include "irrlicht.h"
#include "menu.h"



class COptionsMenu : public CMenu
{
public:
	COptionsMenu(irr::IrrlichtDevice *graphics);
	~COptionsMenu();
	void run();
	bool getApply();
	
	bool getWindowed();
	int getResolution();
private:
	irr::IrrlichtDevice *graphics;
	gui::IGUICheckBox *windowed;
	gui::IGUIComboBox *resolution;

	gui::IGUIButton *apply;
	gui::IGUIButton *close;
};

#endif
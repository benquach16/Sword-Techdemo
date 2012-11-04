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
#include "options_menu.h"

COptionsMenu::COptionsMenu(irr::IrrlichtDevice *graphics) : CMenu(graphics), graphics(graphics)
{
	resolution = graphics->getGUIEnvironment()->addComboBox(rect<s32>(10,10,200,40),getWindow());
	resolution->addItem(L"800x640");
	resolution->addItem(L"1024x768");
	resolution->addItem(L"1280x800");
	resolution->addItem(L"1366x768");
	resolution->addItem(L"1680x1050");
	windowed = graphics->getGUIEnvironment()->addCheckBox(true,rect<s32>(10,50,20,60),getWindow(),-1,L"Windowed");
	graphics->getGUIEnvironment()->addStaticText(L"Windowed",rect<s32>(30,50,200,90),false,true,getWindow());
	apply = graphics->getGUIEnvironment()->addButton(rect<s32>(
		10,getWindowSize().getHeight()-50,140,getWindowSize().getHeight()-10),
		getWindow(),-1,L"Apply");

	close =  graphics->getGUIEnvironment()->addButton(rect<s32>(
		160,getWindowSize().getHeight()-50,340,getWindowSize().getHeight()-10),
		getWindow(),-1,L"Close");


}

void COptionsMenu::run()
{
	if(this->isVisible())
	{
		if(close->isPressed())
		{
			//disable
			this->setVisible(false);
		}
	}
}

bool COptionsMenu::getApply()
{
	if(apply->isPressed())
		return true;
	return false;
}

bool COptionsMenu::getWindowed()
{
	return windowed->isChecked();
}

int COptionsMenu::getResolution()
{
	return resolution->getSelected();
}

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

#ifndef _KEYLISTENER_H_
#define _KEYLISTENER_H_

#include "irrlicht.h"

#pragma once

using namespace irr;
using namespace core;

enum MOUSE_TYPES
{
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_LEFT,
};

class CKeyListener : public irr::IEventReceiver
{
public:


	virtual bool OnEvent(const SEvent& event)
	{
		if(event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
		}

		if(event.EventType==irr::EET_MOUSE_INPUT_EVENT)
		{
			if(event.MouseInput.Event==irr::EMIE_MOUSE_MOVED)
			{
				// check the mouse coords
				mouse.X = event.MouseInput.X;
				mouse.Y = event.MouseInput.Y;
			}
			if(event.MouseInput.Event==irr::EMIE_MOUSE_WHEEL)
			{
				// how do i make it work?
				mouse.wheel += event.MouseInput.Wheel;
			}

			if(event.MouseInput.Event==irr::EMIE_LMOUSE_PRESSED_DOWN)
			{
				if(mouseL!=true)
				{
					mouseL=true;
				}
			}
			if(event.MouseInput.Event==irr::EMIE_LMOUSE_LEFT_UP)
			{
				//mouse released
				if(mouseL!=false)
				{
					mouseL=false;
				}
			}
			if(event.MouseInput.Event==irr::EMIE_RMOUSE_PRESSED_DOWN)
			{
				if(mouseR!=true)
				{
					mouseR=true;
				}
			}
			if(event.MouseInput.Event==irr::EMIE_RMOUSE_LEFT_UP)
			{
				if(mouseR!=false)
				{
					mouseR=false;
				}
			}
			if(event.MouseInput.Event==irr::EMIE_MMOUSE_LEFT_UP)
			{
				if(mouseM!=false)
					mouseM=false;
			}
			if(event.MouseInput.Event==irr::EMIE_MMOUSE_PRESSED_DOWN)
			{
				if(mouseM!=true)
					mouseM=true;
			}
		}
		return false;
	}
	virtual bool IsKeyDown(EKEY_CODE key_code) const
	{
		return KeyIsDown[key_code];
	}
	float mouseX()
	{
		return mouse.X;
	}
	float mouseY()
	{
		return mouse.Y;
	}
	int mouseWheel()
	{
		return mouse.wheel;
	}
	virtual bool IsMouseButtonDown(MOUSE_TYPES type)
	{
		if(type == MOUSE_RIGHT)
			return mouseR;
		if(type == MOUSE_LEFT)
			return mouseL;
		if(type == MOUSE_MIDDLE)
			return mouseM;
	}
	
	CKeyListener()
	{
		//set everything to false
		//all variables must be initialized first
		mouseL = false;
		mouseR = false;
		mouseM = false;
		for(int i=0; i<irr::KEY_KEY_CODES_COUNT; i++)
		{
			KeyIsDown[i] = false;
		}
	}
private:
	//stores all the keys in a bool array
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool mouseL;
	bool mouseR;
	bool mouseM;
	struct mouseCoords
	{
		float X;
		float Y;
		float wheel;
	};
	struct mouseCoords mouse;
};

#endif
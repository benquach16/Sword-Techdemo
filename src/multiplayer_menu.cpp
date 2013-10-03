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
#include "multiplayer_menu.h"

CMultiplayerMenu::CMultiplayerMenu(irr::IrrlichtDevice *graphics) : CMenu(graphics), graphics(graphics)
{
	exit = graphics->getGUIEnvironment()->addButton(rect<s32>(
		getWindowSize().getWidth()-150,getWindowSize().getHeight()-50,getWindowSize().getWidth()-10,getWindowSize().getHeight()-10),
		getWindow(),-1,L"Exit");
	join = graphics->getGUIEnvironment()->addButton(rect<s32>(
		10,getWindowSize().getHeight()-50,140,getWindowSize().getHeight()-10),
		getWindow(),-1,L"Join");
	create_server = graphics->getGUIEnvironment()->addButton(rect<s32>(
		160,getWindowSize().getHeight()-50,340,getWindowSize().getHeight()-10),
		getWindow(),-1,L"Create Server");

	server_ip = graphics->getGUIEnvironment()->addEditBox(L"", rect<s32>(10,50,250,80),true,getWindow());
	player_name = graphics->getGUIEnvironment()->addEditBox(L"", rect<s32>(270,50,500,80),true,getWindow());
	graphics->getGUIEnvironment()->addStaticText(L"Player Name :", rect<s32>(270,100,500,130),false,true,getWindow());
	your_ip = graphics->getGUIEnvironment()->addStaticText(L"Your IP :", rect<s32>(10,100,250,130), false, true, getWindow());
	graphics->getGUIEnvironment()->addStaticText(L"Peer-to-Peer (needed if hosting behind router)", rect<s32>(
		180,getWindowSize().getHeight()-90,540,getWindowSize().getHeight()-50),false,true,getWindow());
	router = graphics->getGUIEnvironment()->addCheckBox(false,rect<s32>(
		160,getWindowSize().getHeight()-70,170,getWindowSize().getHeight()-60),getWindow());


	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	for (unsigned int i=0; i < peer->GetNumberOfAddresses(); i++)
	{
		printf("%i. %s\n", i+1, peer->GetLocalIP(i));

	}
	const char *ip = peer->GetLocalIP(peer->GetNumberOfAddresses());
	stringw c = L"";
	c+=ip;
	your_ip->setText(c.c_str());
}

CMultiplayerMenu::~CMultiplayerMenu()
{
}

void CMultiplayerMenu::run()
{
	if(this->isVisible())
	{
		if(exit->isPressed())
		{
			//disable
			this->setVisible(false);
		}
	}
}

bool CMultiplayerMenu::getRunClient()
{
	if(join->isPressed())
	{
		return true;
	}
	return false;
}

bool CMultiplayerMenu::getRunServer()
{
	if(create_server->isPressed())
		return true;
	return false;
}

char *CMultiplayerMenu::getServerIp()
{
	stringw id(L"");
	id += server_ip->getText();
	int size_id =id.size();
	char *buffer_id;
	buffer_id = new char[size_id+1];
	//re encode the string onto the new space
	for(int n=0;n<size_id;n++)
	{
		buffer_id[n]=id[n];
	}
	//termination character
	buffer_id[size_id]='\0';

	return buffer_id;
}

const wchar_t *CMultiplayerMenu::getServerText()
{
	return server_ip->getText();
}

const char *CMultiplayerMenu::getPlayerName()
{
	stringw id(L"");
	id += player_name->getText();
	int size_id =id.size();
	char *buffer_id;
	buffer_id = new char[size_id+1];
	//re encode the string onto the new space
	for(int n=0;n<size_id;n++)
	{
		buffer_id[n]=id[n];
	}
	//termination character
	buffer_id[size_id]='\0';

	return buffer_id;
}

bool CMultiplayerMenu::getRouter()
{
	return router->isChecked();
}
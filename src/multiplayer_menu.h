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

#ifndef _MULTIPLAYER_MENU_H_
#define _MULTIPLAYER_MENU_H_

#include "menu.h"
#include "RakPeerInterface.h"
#include "stdlib.h"
#include "stdio.h"

class CMultiplayerMenu : public CMenu
{
public:
	CMultiplayerMenu(irr::IrrlichtDevice *graphics);
	void run();
	bool getRunServer();
	bool getRunClient();
	~CMultiplayerMenu();
	char *getServerIp();
	bool getRouter();
	const wchar_t *getServerText();
	const char *getPlayerName();
private:
	irr::IrrlichtDevice *graphics;
	
	gui::IGUIButton *exit;
	gui::IGUIButton *join;
	gui::IGUIButton *create_server;
	gui::IGUIEditBox *player_name;
	gui::IGUIEditBox *server_ip;
	gui::IGUIStaticText *your_ip;
	gui::IGUICheckBox *router;


};

#endif
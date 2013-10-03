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

//init.h is the starting part of the program
//initializes the libararies and the main graphics windows


#ifndef _INIT_H_
#define _INIT_H_

#pragma once

#include "irrlicht.h"
#include "irrklang.h"
#include "gameloop.h"
#include "keylistener.h"
#include "stdio.h"
#include "stdlib.h"
#include "multiplayer_menu.h"
#include "options_menu.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "NatPunchthroughServer.h"
#include "NatPunchthroughClient.h"
#include "CloudClient.h"
#include "FullyConnectedMesh2.h"
#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"
#include "UDPProxyClient.h"
#include "TCPInterface.h"
#include "HTTPConnection.h"
#include "Newton.h"
#include "windows.h"
#include "sstream"
#include "iostream"
#include "fstream"
#include "strstream"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;
using namespace RakNet;
using namespace std;

//use as return values in order to see
//if the client can connect to the server
enum CONNECTION_TYPE
{
	TYPE_CONNECTED,
	TYPE_SERVER_FULL,
	TYPE_COULD_NOT_CONNECT,
};



class CInit
{
public:
	//constructor
	CInit();
	//main menu
	void mainMenuRun();
	//main graphcis loop
	void run();
	//destructor
	~CInit();
	//join as client
	CONNECTION_TYPE joinServer();
	//create new internet server
	void createServer();
	//plugnplay, hopefully it works
	//other misc functions
	void readVideoFile();
	//create new video file
	void generateVideoFile(int x, int y, bool windowed);

private:
	//main graphics window and irrlicht library
	irr::IrrlichtDevice *graphics;
	//irrklang library
	irrklang::ISoundEngine *sound;
	//keylistener to get key input
	CKeyListener *keylistener;
	//main game loop
	CGameloop *Game;
	//physics world
	NewtonWorld *world;

	//menus
	CMultiplayerMenu *multiplayer_menu;
	COptionsMenu *options_menu;
	
	//network
	//important variables
	bool isSingleplayer;
	bool isServer; 
	char ip_joined;

	//defines the machine for the network code
	RakNet::RakPeerInterface *current_machine;
	//not used
	RakNet::NatPunchthroughServer *punchthrough_server;
	//client used to punchthrough the router port
	RakNet::NatPunchthroughClient *punchthrough_client;

	
};

#endif
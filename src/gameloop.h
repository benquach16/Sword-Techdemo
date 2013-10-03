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


//main loop file
//everything is run from here

#ifndef _GAMELOOP_H_
#define _GAMELOOP_H

#include "irrlicht.h"
#include "irrklang.h"
#include "Newton.h"
#include "person.h"
#include "player.h"
#include "camera.h"
#include "hud.h"
#include "gamemanager.h"
#include "keylistener.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "windows.h"
#include "BitStream.h"
#include "RakMemoryOverride.h"
#include "NatPunchthroughServer.h"
#include "NatPunchthroughClient.h"
#include "FullyConnectedMesh2.h"
#include "CloudClient.h"
#include "RakNetSmartPtr.h"
#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"
#include "UDPProxyClient.h"
#include "TCPInterface.h"
#include "HTTPConnection.h"
#include "Itoa.h"
#include "GetTime.h"
#include "stdio.h"

#pragma once

using namespace irr;
using namespace scene;
using namespace core;
using namespace video;
using namespace RakNet;

enum NetworkMessage
{
	ID_PLAYER_DATA = ID_USER_PACKET_ENUM+1,
	ID_MESSAGE_DATA = ID_PLAYER_DATA+1,
};

class CGameloop
{
public:
	CGameloop(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, 
		CKeyListener *keylistener, NewtonWorld *world, bool singleplayer, 
		RakNet::RakPeerInterface *current_machine, bool isServer, const char* name,
		RakNet::RakNetGUID serverGUID, RakNet::NatPunchthroughClient *punchthrough_client);
	void run();
	void playerControl(f32 frameDeltaTime);
	void cameraControl();
	void singleplayerSetup();
	void multiplayerSetup();
	//need to generate newton polygon meshes
	//if you're a boss, you can optimize the meshes through CODE
	NewtonCollision *createNCollisionMesh(NewtonWorld *world, scene::IMesh *mesh);

	//network code goes here
	void loadData(Packet *p);
	void sendPlayerData(RakNet::SystemAddress address);
	bool findUPNP();
	~CGameloop();

	bool getExit()
	{
		return exit_;
	}
	void setExit(bool exit)
	{
		exit_ = exit;
	}


private:
	irr::IrrlichtDevice *graphics;
	irrklang::ISoundEngine *sound;
	NewtonWorld *world;
	RakNet::RakPeerInterface *current_machine;

	scene::IAnimatedMeshSceneNode *room;
	NewtonCollision *room_coll;
	NewtonBody *room_body;
	scene::ICameraSceneNode *p_cam;
	SObject *room_object;

	CCamera *cam;
	CPlayer *player;	//player node
	CPlayer *client;	//temporary only; should be done dynamically
	CPerson *man;

	//important gamerun objects
	CKeyListener *keylistener;
	CGameManager *game_manager;
	CHud *hud;


	u32 then;
	f32 frameDeltaTime;

	bool exit_;		//for exiting
	//networking variables
	//
	bool isServer;
	bool isSingleplayer;
	const char *name_;
	RakNet::NatPunchthroughClient *punchthrough_client;
	RakNet::NatPunchthroughClient *clientn;
	CloudClient *cloudClient;
	RakNet::SystemAddress clientID;		//for multiple people use an array
	RakNet::RakNetGUID serverGUID;		//for NAT
	RakNet::Packet *packet;
	RakNet::FullyConnectedMesh2 *fullyconnectedmesh;

	
};

#endif
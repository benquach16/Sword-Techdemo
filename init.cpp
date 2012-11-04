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
#include "init.h"

//using irrlicht for graphics, ngd for physics, irrklang for sound, raknet for networking
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(lib, "newton.lib")
#pragma comment(lib, "irrKlang.lib")
#pragma comment(lib, "RakNetLibStatic.lib")
#pragma comment(lib, "miniupnpc.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#define MAX_CLIENTS 2
#define PORT 60000
#define DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_PORT 61111
#define DEFAULT_NAT_PUNCHTHROUGH_FACILITATOR_IP "94.198.81.195"
#define DEFAULT_X 800
#define DEFAULT_Y 600
#define DEFAULT_WINDOWED 1
#define STATICLIB

//define globals to be modified by the video ini
core::dimension2d<s32> t;
int GWINDOWX;
int GWINDOWY;
bool GWINDOWED;

void CInit::readVideoFile()
{
	ifstream file("res/video.ini");
	std::vector<std::string> container;
	if(file)
	{
		if(file.is_open())
		{
			std::string str;
			getline(file,str);

			std::istringstream sstream(str);
			//get everything in between semicolon
			while(sstream)
			{
				std::string s;

				if(!getline(sstream, s, ';'))
					break;
				container.push_back(s);
			}
		}
		GWINDOWX = atoi(container[0].c_str());
		GWINDOWY = atoi(container[1].c_str());
		GWINDOWED = atoi(container[2].c_str());
	}
	else
	{
		//no file exists? generate one
		generateVideoFile(DEFAULT_X, DEFAULT_Y, DEFAULT_WINDOWED);
	}
}

void CInit::generateVideoFile(int x, int y, bool windowed)
{
	ifstream file("res/video.ini");
	if(file)
	{
		//delete
		remove("res/video.ini");
		ofstream savefile;
		savefile.open("res/video.ini");
		if(windowed == true)
			savefile << x << ";" << y << ";" << "1";
		else
			savefile << x << ";" << y << ";" << "0";
	}
}


CInit::CInit()
{

	//first read config
	readVideoFile();
	
	//create the graphics driver
	graphics = createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(GWINDOWX,GWINDOWY),32,GWINDOWED,false,false,0);
	t = graphics->getVideoDriver()->getScreenSize();

	//initialize key receiver
	keylistener = new CKeyListener;
	graphics->setEventReceiver(keylistener);

	//set the font for the environment
	gui::IGUIFont *font = graphics->getGUIEnvironment()->getFont("res/fonts/basic.xml");
	graphics->getGUIEnvironment()->getSkin()->setFont(font);

	//create newton world
	//initalize basic newton variables, although newton is used for collision detection only
	world = NewtonCreate();
	NewtonSetPlatformArchitecture(world, 0);
    NewtonSetSolverModel(world, 1);
	int i = NewtonMaterialGetDefaultGroupID(world);
	NewtonMaterialSetDefaultFriction (world, i, i, 0.8f, 0.4f);
	NewtonMaterialSetDefaultElasticity (world, i, i, 0.3f);
	NewtonMaterialSetDefaultSoftness (world, i, i, 0.05f);
	NewtonMaterialSetCollisionCallback (world, i, i, NULL, NULL, NULL);
	NewtonSetSolverModel(world, 0);
	NewtonSetFrictionModel(world, 0);
	//create sound engine
	sound = irrklang::createIrrKlangDevice();
	sound->setDefault3DSoundMinDistance(400);

	
	//create an instance of the rakpeer class
	current_machine = RakNet::RakPeerInterface::GetInstance();

	//create game menus
	multiplayer_menu = new CMultiplayerMenu(graphics);
	multiplayer_menu->setVisible(false);
	options_menu = new COptionsMenu(graphics);
	options_menu->setVisible(false);

	mainMenuRun();
}

void CInit::mainMenuRun()
{
	//Render loop for the main menu
	//The render loop for the main game is in a seperate func
	

	//create GUI buttons
	gui::IGUIButton *singleplayer = graphics->getGUIEnvironment()->addButton(rect<s32>(
		t.Width/2-90,t.Height/2-60,t.Width/2+90,t.Height/2-30),0,-1,L"Singleplayer");
	gui::IGUIButton *multiplayer = graphics->getGUIEnvironment()->addButton(rect<s32>(
		t.Width/2-90,t.Height/2-20,t.Width/2+90,t.Height/2+10),0,-1,L"Multiplayer");
	gui::IGUIButton *options = graphics->getGUIEnvironment()->addButton(rect<s32>(
		t.Width/2-90,t.Height/2+20,t.Width/2+90,t.Height/2+50),0,-1,L"Options");
	gui::IGUIButton *quit = graphics->getGUIEnvironment()->addButton(rect<s32>(
		t.Width/2-90,t.Height/2+60,t.Width/2+90,t.Height/2+90),0,-1,L"Quit");

	bool start = false;
	while(graphics->run())
	{
		//main graphics loop
		graphics->getVideoDriver()->beginScene(true,true,SColor(255,101,100,140));

		graphics->getSceneManager()->drawAll();

		graphics->getVideoDriver()->draw2DImage(graphics->getVideoDriver()->getTexture("res/textures/bkg.png"),rect<s32>(
			0,0,graphics->getVideoDriver()->getScreenSize().Width,graphics->getVideoDriver()->getScreenSize().Height), rect<s32>(
			0,0,800,600));
		graphics->getGUIEnvironment()->drawAll();

		//run game loops here
		//menus
		multiplayer_menu->run();
		options_menu->run();

		//button check
		if(singleplayer->isPressed())
		{
			start = true;
			isSingleplayer = true;
			isServer = false;
			break;
		}
		//multiplayer
		if(multiplayer->isPressed())
		{
			multiplayer_menu->setVisible(true);
		}
		if(multiplayer_menu->getRunClient())
		{
			isSingleplayer = false;
			isServer = false;
			multiplayer_menu->setVisible(false);
			if(joinServer()== TYPE_CONNECTED)
			{
				//connected to server
				start = true;
				break;
			}
			if(joinServer()==TYPE_COULD_NOT_CONNECT)
			{
				graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Could not connect to the server");
			}
			if(joinServer()==TYPE_SERVER_FULL)
			{
				graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Server is full");
			}

		}
		if(multiplayer_menu->getRunServer())
		{
			isSingleplayer = false;
			isServer = true;
			multiplayer_menu->setVisible(false);
			createServer();
			start = true;
			break;
		}
		//options
		if(options->isPressed())
		{
			options_menu->setVisible(true);
		}
		if(options_menu->getApply())
		{
			//write ini
			if(options_menu->getResolution()==0)
				this->generateVideoFile(800,600,options_menu->getWindowed());
			if(options_menu->getResolution()==1)
				this->generateVideoFile(1024,768,options_menu->getWindowed());
			if(options_menu->getResolution()==2)
				this->generateVideoFile(1280,800,options_menu->getWindowed());
			if(options_menu->getResolution()==3)
				this->generateVideoFile(1366,768,options_menu->getWindowed());
			if(options_menu->getResolution()==4)
				this->generateVideoFile(1680,1050,options_menu->getWindowed());
		}

		if(quit->isPressed())
		{
			//quit button pressed
			break;
		}

		graphics->getVideoDriver()->endScene();
	}
	singleplayer->remove();
	multiplayer->remove();
	options->remove();
	quit->remove();
	if(start == true)
		run();
}

CONNECTION_TYPE CInit::joinServer()
{
	//primarily used by me to see if raknet works
	//graphics->setWindowCaption(L"Client");	
	SocketDescriptor sd;
	//initialize client
	current_machine->Startup(MAX_CLIENTS+1, &sd, 1);
	current_machine->SetMaximumIncomingConnections(MAX_CLIENTS);
	stringc str = multiplayer_menu->getServerIp();
	RakNet::ConnectionAttemptResult result;
	//initialize nat puncthrough if the client requests to use nat puncthrough
	//in order to connect to a server behind a router
	if(multiplayer_menu->getRouter())
	{
		punchthrough_client = new NatPunchthroughClient;
		current_machine->AttachPlugin(punchthrough_client);

		result = current_machine->Connect("94.198.81.195", 61111, 0, 0);
	}
	else
		result = current_machine->Connect(str.c_str(), 60000, 0,0);
	//test to see if can connect

	//Sleep is a WINDOWS FUNCTION
	//FOR CROSS COMPATIBILITY WITH OTHER OSES, DO NOT USE Sleep()
	Sleep(1000);
    switch(result)
    {
		//most of this results in the client not being able to connect
        case RakNet::INVALID_PARAMETER:
            return TYPE_COULD_NOT_CONNECT;
        break;
        case RakNet::CANNOT_RESOLVE_DOMAIN_NAME:
            return TYPE_COULD_NOT_CONNECT;
        break;
        case RakNet::ALREADY_CONNECTED_TO_ENDPOINT:
            return TYPE_COULD_NOT_CONNECT;
        break;
        case RakNet::CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
            return TYPE_COULD_NOT_CONNECT;
        break;
        case RakNet::SECURITY_INITIALIZATION_FAILED:
            return TYPE_COULD_NOT_CONNECT;
        break;
    }
	if(current_machine->IsActive())
		return TYPE_CONNECTED;
	return TYPE_COULD_NOT_CONNECT;
}

void CInit::createServer()
{
	//is the servers

	graphics->setWindowCaption(L"Server");
	SocketDescriptor sd(60000, 0);
	current_machine->Startup(MAX_CLIENTS+1, &sd, 1);
	current_machine->SetMaximumIncomingConnections(MAX_CLIENTS);
	if(multiplayer_menu->getRouter())
	{

		punchthrough_client = new NatPunchthroughClient;
		current_machine->AttachPlugin(punchthrough_client);
		//punchthrough_server = new NatPunchthroughServer;
		//current_machine->AttachPlugin(punchthrough_server);
		current_machine->Connect("94.198.81.195", 61111, 0, 0);
		//TCPInterface tcp;
		//tcp.Start(0,64);

	}
}


void CInit::run()
{

	stringc str = multiplayer_menu->getPlayerName();
	RakNet::RakNetGUID guid;
	stringc strip = multiplayer_menu->getServerIp();
	guid.FromString(strip.c_str());

	//create the CGameloop object
	//loop through it in order to run the game

	Game = new CGameloop(graphics,sound,keylistener,world,isSingleplayer,current_machine, isServer, str.c_str(), guid, punchthrough_client);
	while(graphics->run())
	{
		//main graphics loop
		graphics->getVideoDriver()->beginScene(true,true,SColor(255,101,100,140));

		
		graphics->getSceneManager()->drawAll();

		graphics->getGUIEnvironment()->drawAll();
		//run game loops here
		
		Game->run();
		NewtonUpdate(world, 0.05f);		

		if(Game->getExit()==true)
			break;

		graphics->getVideoDriver()->endScene();
	}
 }

CInit::~CInit()
{
	::NewtonDestroy(world);
	current_machine->Shutdown(1000);
	RakNet::RakPeerInterface::DestroyInstance(current_machine);
	//current_machine = NULL;
}
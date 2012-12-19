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
#include "gameloop.h"

#define STATICLIB
#define SUPPORT_UPNP PENDING
#define SUPPORT_NAT_TYPE_DETECTION PENDING
#define SUPPORT_NAT_PUNCHTHROUGH PENDING
#define SUPPORT_ROUTER2 PENDING
#define SUPPORT_UDP_PROXY PENDING


//used to turn
float tMouseX = 0;
float tMouseY = 0;
vector2d<u32> screen_center;

//main gameplay loop
CGameloop::CGameloop(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound,
	CKeyListener *keylistener, NewtonWorld *world, bool singleplayer,
	RakNet::RakPeerInterface *current_machine, bool isServer, const char *name, 
	RakNet::RakNetGUID serverGUID, RakNet::NatPunchthroughClient *punchthrough_client) : 
graphics(graphics), sound(sound), world(world), keylistener(keylistener), current_machine(current_machine), exit_(false), 
	isSingleplayer(singleplayer), isServer(isServer), name_(name), serverGUID(serverGUID), punchthrough_client(punchthrough_client)
{
	screen_center = graphics->getVideoDriver()->getScreenSize();
	screen_center = screen_center/2;

	cloudClient = new CloudClient;
	current_machine->AttachPlugin(cloudClient);
	fullyconnectedmesh = new FullyConnectedMesh2;
	fullyconnectedmesh->SetAutoparticipateConnections(false);
	fullyconnectedmesh->SetConnectOnNewRemoteConnection(false, "");
	current_machine->AttachPlugin(fullyconnectedmesh);
	//initialize variables
	then = graphics->getTimer()->getTime();


	game_manager = new CGameManager(graphics, sound, world);
	hud = new CHud(graphics);
	scene::ITriangleSelector *selector;

	//create room
	room_object = new SObject;

	room = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/room.x"));

	room->setScale(vector3df(0.2,0.2,0.2));
	room->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);
	room->getMaterial(0).SpecularColor.set(0,0,0,0);
	room_object->mesh = room;
	room_coll = createNCollisionMesh(world, room->getMesh());	
	room_object->coll = room_coll;
	room_body = NewtonCreateBody(world,room_coll,(float*)&room->getAbsoluteTransformation());

	float boxP0[3]; 
	float boxP1[3]; 
	float matrix[4][4]; 
	NewtonBodyGetMatrix (room_body, &matrix[0][0]); 
	NewtonCollisionCalculateAABB (room_coll, &matrix[0][0],  &boxP0[0], &boxP1[0]); 
	NewtonSetWorldSize (world, (float*)boxP0, (float*)boxP1);


	//create player
	int i = rand()%200;
	player = new CPlayer(graphics, world, vector3df(i,0,-i),vector3df(0,0,0), name_);  
	game_manager->addPerson(player);
	client = 0;

	//setup misc
	p_cam = graphics->getSceneManager()->addCameraSceneNode();
	p_cam->bindTargetAndRotation(true);
	int degree = 90;
	float rad = degree * 3.14 / 180;
	p_cam->setFOV(rad);
	p_cam->setFarValue(40000);

	graphics->getCursorControl()->setPosition(0.5f,0.5f);
	graphics->getSceneManager()->addLightSceneNode(0,vector3df(0,150,0), SColor(255,255,255,255), 200);
	graphics->getSceneManager()->setAmbientLight(SColor(74,74,74,74));
	graphics->getSceneManager()->setShadowColor();

	if(singleplayer==true)
		singleplayerSetup();
}

CGameloop::~CGameloop()
{

}

void CGameloop::singleplayerSetup()
{
	//create generic bad guy
	man = new CPerson(graphics, world, vector3df(-100,0,100));

	game_manager->addPerson(man);
}

void CGameloop::multiplayerSetup()
{

}

void CGameloop::loadData(Packet *p)
{
	//decode the datastream
	//into useful information
	//look at the sendData func to see the structure of the datastream
	BitStream in(p->data,p->length,false);
	RakString name;
	MessageID tmp;
	in.Read(tmp);
	in.Read(name);
	float x,y,z;
	in.Read(x);
	in.Read(y);
	in.Read(z);
	float rot_x, rot_y, rot_z;
	in.Read(rot_x);
	in.Read(rot_y);
	in.Read(rot_z);
	float rs_x,rs_y,rs_z;
	in.Read(rs_x);
	in.Read(rs_y);
	in.Read(rs_z);
	bool slash_left,slash_right,slash_top;
	in.Read(slash_left);
	in.Read(slash_right);
	in.Read(slash_top);
	int swing_force;
	in.Read(swing_force);
	bool under_block;
	in.Read(under_block);
	if(client == 0)
	{

		client = new CPlayer(graphics,world,vector3df(x,y,z),vector3df(rot_x,rot_y,rot_z),name.C_String());
		game_manager->addPerson(client);
	}
	else
	{
		client->setPosition(vector3df(x,y,z));
		client->setRotation(vector3df(rot_x,rot_y,rot_z));
		client->setRShoulderRotation(vector3df(rs_x,rs_y,rs_z));
		client->setSlashLeft(slash_left);
		client->setSlashRight(slash_right);
		client->setSlashTop(slash_top);
		client->setSwingForce(swing_force);
		client->setUnderBlock(under_block);
		//client->setVisible(true);
	}
	
}

void CGameloop::sendPlayerData(RakNet::SystemAddress address)
{
	//looks mostly like a block of code
	//creates a datastream and writes all the data to the
	//datastream
	//then the datastream is sent over the internet
	//to the server
	RakNet::BitStream bit;
	bit.Write((MessageID)ID_PLAYER_DATA);
	bit.Write(player->getName());
	bit.Write(player->getPosition().X);
	bit.Write(player->getPosition().Y);
	bit.Write(player->getPosition().Z);
	bit.Write(player->getRotation().X);
	bit.Write(player->getRotation().Y);
	bit.Write(player->getRotation().Z);
	bit.Write(player->getRShoulderRotation().X); 
	bit.Write(player->getRShoulderRotation().Y);
	bit.Write(player->getRShoulderRotation().Z);
	bit.Write(player->getSlashLeft());
	bit.Write(player->getSlashRight());
	bit.Write(player->getSlashTop());
	bit.Write(player->getSwingForce());
	bit.Write(player->getUnderBlock());
	current_machine->Send(&bit, HIGH_PRIORITY, RELIABLE_ORDERED, 0, address, false);
}

bool CGameloop::findUPNP()
{
	// Open UPNP.
	struct UPNPDev * devlist = 0;
	devlist = upnpDiscover(2000, 0, 0, 0, 0, 0);
	if (devlist)
	{
		char lanaddr[64];	/* my ip address on the LAN */
		struct UPNPUrls urls;
		struct IGDdatas data;
		if (UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==1)
		{
			DataStructures::List< RakNetSmartPtr< RakNetSocket> > sockets;
			current_machine->GetSockets(sockets);

			char iport[32];
			Itoa(sockets[0]->boundAddress.GetPort(),iport,10);
			char eport[32];
			Itoa(current_machine->GetExternalID(SystemAddress("94.198.81.195", 61111)).GetPort(),eport,10);
			int r = UPNP_AddPortMapping(urls.controlURL, data.first.servicetype, eport, iport, lanaddr, 0, "UDP", 0, "0");
			if(r!=UPNPCOMMAND_SUCCESS)
			{
				// UPNP done
				return false;
			}

		}
		else
		{
			//THIS IS WHAT RETURNS FALSE
			//THIS IS THE CAUSE OF ALL MY PROBLEMS
			if(UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==0)
				graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Router does not support plug and play. Use a dedicated Server");
			if(UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==2)
				graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Router UPnP not connected, please configure it");
			if(UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))==3)
				graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Your router is not recognized");
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void CGameloop::run()
{
	
	const u32 now = graphics->getTimer()->getTime();
	f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	frameDeltaTime = 0.005;
	then = now;

	sound->setListenerPosition(player->getPosition(),player->getHeadRotation());
	game_manager->run(frameDeltaTime);
	playerControl(frameDeltaTime);
	hud->run(player);
	cameraControl();

	//receive packets if not singleplayer
	if(isSingleplayer == false)
	{
		//multiplayer update
		for (packet=current_machine->Receive(); packet; current_machine->DeallocatePacket(packet), packet=current_machine->Receive())
		{
			//psuedo server for nat punchthrough, due to the fact that i cant connect with my normal server
			switch (packet->data[0])
			{
				//NAT server is incompatible with this version
				//create a new update with new raknet ver
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection", L"Invalid Protocol");
					if (packet->systemAddress==SystemAddress("94.198.81.195", 61111))
						graphics->getGUIEnvironment()->addMessageBox(L"Connection", L"Requires NAT server");
				}
				break;
			case ID_NAT_PUNCHTHROUGH_FAILED:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"NAT Failed");
				}
				break;
			case ID_NAT_PUNCHTHROUGH_SUCCEEDED:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection", L"NAT Success");
					current_machine->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(), 0, 0);
				}
				break;
			case ID_NAT_TARGET_NOT_CONNECTED:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection", L"Server not connected to NAT");
				}
				break;
			case ID_NAT_TARGET_UNRESPONSIVE:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection", L"NAT Server is Unavailable");
				}
				break;
			case ID_NAT_CONNECTION_TO_TARGET_LOST:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection", L"NAT Lost Connection");
				}
				break;
			case ID_NAT_ALREADY_IN_PROGRESS:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection", L"NAT Already Started");
				}
				break;
			case ID_PLAYER_DATA:
				{
					loadData(packet);					
				}
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Server was full");
				}
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Could not connect to the server");
				}
				break;
			case ID_NEW_INCOMING_CONNECTION:
				{
					if (fullyconnectedmesh->IsHostSystem())
					{
						fullyconnectedmesh->StartVerifiedJoin(packet->guid);
					}
				}
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				{
					if(packet->systemAddress == SystemAddress("94.198.81.195", 61111))
					{
						//connected to NAT server		

						if(findUPNP()==true)
						{
							// Query cloud for other running game instances
							RakNet::CloudQuery cloudQuery;
							cloudQuery.keys.Push(RakNet::CloudKey("sword",0),_FILE_AND_LINE_);
							cloudClient->Get(&cloudQuery, packet->guid);
						}
						else
						{
							//UPNP was not successful
							//no way to find router ports now
						}
					}
				}
			case ID_ADVERTISE_SYSTEM:
				if (packet->guid!=current_machine->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS))
				{
					char hostIP[32];
					packet->systemAddress.ToString(false,hostIP);
					current_machine->Connect(hostIP,packet->systemAddress.GetPort(),0,0);
				}
				break;
			case ID_FCM2_NEW_HOST:
				{
					if (packet->guid==current_machine->GetMyGUID())
					{
						// Original host dropped. I am the new session host. Upload to the cloud so new players join this system.
						RakNet::CloudKey cloudKey("sword",0);
						cloudClient->Post(&cloudKey, 0, 0, current_machine->GetGuidFromSystemAddress(SystemAddress("94.198.81.195", 61111)));
					}
				}
				break;
			case ID_FCM2_VERIFIED_JOIN_FAILED:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"Could not connect to the server");
				}
				break;
			case ID_FCM2_VERIFIED_JOIN_CAPABLE:
				{
					fullyconnectedmesh->RespondOnVerifiedJoinCapable(packet, true, 0);
				}
				break;
			case ID_FCM2_VERIFIED_JOIN_START:
				{
					DataStructures::List<RakNet::SystemAddress> addresses;
					DataStructures::List<RakNet::RakNetGUID> guids;
					fullyconnectedmesh->GetVerifiedJoinRequiredProcessingList(packet->guid, addresses, guids);
					for (unsigned int i=0; i < guids.Size(); i++)
						punchthrough_client->OpenNAT(guids[i], SystemAddress("94.198.81.195", 61111));
				}
				break;
			case ID_CLOUD_GET_RESPONSE:
				{

					RakNet::CloudQueryResult cloudQueryResult;
					cloudClient->OnGetReponse(&cloudQueryResult, packet);
					if (cloudQueryResult.rowsReturned.Size()>0)
					{	
						punchthrough_client->OpenNAT(cloudQueryResult.rowsReturned[0]->clientGUID, SystemAddress("94.198.81.195", 61111));
					}
					else
					{
						RakNet::CloudKey cloudKey("sword",0);
						cloudClient->Post(&cloudKey, 0, 0, packet->guid);
						graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"No servers available");

					}

					cloudClient->DeallocateWithDefaultAllocator(&cloudQueryResult);
				}
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"A Player Disconnected");
				}
				break;
			case ID_REMOTE_CONNECTION_LOST:
				{
					graphics->getGUIEnvironment()->addMessageBox(L"Connection Problem", L"A Player has lost Connection");
				}
				break;
			case ID_ROUTER_2_FORWARDING_ESTABLISHED:
				{
					RakNet::BitStream bs(packet->data, packet->length, false);
					bs.IgnoreBytes(sizeof(MessageID));
					RakNetGUID endpointGuid;
					char str[64];
					bs.Read(endpointGuid);
					printf("Routing through %s to %s successful. Connecting.\n", str, endpointGuid.ToString());
					unsigned short sourceToDestPort;
					bs.Read(sourceToDestPort);
					char ipAddressString[32];
					packet->systemAddress.ToString(false, ipAddressString);
					current_machine->Connect(ipAddressString, sourceToDestPort, 0,0);
				}
				break;
			default:
				{
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					clientID = packet->systemAddress;
				}
				break;
			}
		}
		sendPlayerData(clientID);
	}
}

void CGameloop::playerControl(f32 frameDeltaTime)
{
	//control player

	//movement
	//key inputs go here
	if(keylistener->IsKeyDown(irr::KEY_KEY_W))
	{
		player->moveForward(frameDeltaTime);
	}
	if(keylistener->IsKeyDown(irr::KEY_KEY_S))
	{
		player->moveBackward(frameDeltaTime);
	}
	if(keylistener->IsKeyDown(irr::KEY_KEY_A))
	{
		player->moveLeft(frameDeltaTime);
	}
	if(keylistener->IsKeyDown(irr::KEY_KEY_D))
	{
		player->moveRight(frameDeltaTime);
	}

	//attacking/
	/*
	if(keylistener->IsMouseButtonDown(MOUSE_RIGHT))
	{
		player->setSlashRight(true);
	}
	else
	{
		player->setSlashRight(false);
	}
	if(keylistener->IsMouseButtonDown(MOUSE_LEFT))
	{
		player->setSlashLeft(true);
	}
	else
	{
		player->setSlashLeft(false);
	}
	if(keylistener->IsMouseButtonDown(MOUSE_MIDDLE))
	{
		player->setSlashTop(true);
	}
	else
	{
		player->setSlashTop(false);
	}
	*/

	//new attack system goes here
	if(keylistener->IsMouseButtonDown(MOUSE_LEFT))
	{
		//wherever the mouse is, decides if the player attacks from left or right
		if(!player->getSlashLeft() && !player->getSlashRight() && !player->getSlashTop() && !player->getUnderBlock())
		{
			if(player->getRShoulderRotation().Y < 330 && player->getRShoulderRotation().Y > 180)
			{
				player->setSlashTop(true);
			}
			else
			{
				if(player->getRShoulderRotation().Z > 280)
				{
					player->setSlashRight(true);
				}
				else if(player->getRShoulderRotation().Z < 280)
				{
					player->setSlashLeft(true);
				}
			}
		}
	}
	else
	{
		player->setSlashLeft(false);
		player->setSlashRight(false);
		player->setSlashTop(false);
	}
	if(keylistener->IsMouseButtonDown(MOUSE_RIGHT))
	{
		player->setUnderBlock(true);
	}
	else
	{
		player->setUnderBlock(false);
	}

	if(keylistener->IsKeyDown(irr::KEY_ESCAPE))
	{
		RakNet::BitStream bit;
		bit.Write((MessageID)ID_REMOTE_DISCONNECTION_NOTIFICATION);
		current_machine->Send(&bit, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientID, false);
		setExit(true);
	}
	//rotation 
	
	//mouse position changed
	//update rotation
	if(keylistener->mouseX()!=tMouseX)
	{
		float i=keylistener->mouseX()-tMouseX;
		if(player->getRShoulderRotation().Z > 310 || player->getRShoulderRotation().Z < 250)
			player->rotateFacingY(i);
		tMouseX=keylistener->mouseX();
		if(player->getSwingForce() > 1)
			player->rotateRArmX(i);

	}
	if(keylistener->mouseY()!=tMouseY)
	{
		float t=keylistener->mouseY()-tMouseY;
		//if(player->getRShoulderRotation().Y < 50)
			 player->rotateFacingX(t);
		tMouseY=keylistener->mouseY();
		if(player->getSwingForce() > 1)
			player->rotateRArmY(t);

	}

	//prevent cursor from leaving screen
	if(graphics->getCursorControl()->getRelativePosition().X > 1)
	{
		graphics->getCursorControl()->setPosition(1.f,graphics->getCursorControl()->getRelativePosition().Y);
	}
	if(graphics->getCursorControl()->getRelativePosition().X < 0)
	{
		graphics->getCursorControl()->setPosition(0.f,graphics->getCursorControl()->getRelativePosition().Y);
	}
}

void CGameloop::cameraControl()
{
	vector3df h_rot = player->getRotation();
	h_rot.X = player->getHeadRotation().X;
	player->setHeadRotation(h_rot);
	p_cam->setRotation(player->getHeadRotation());
	
	vector3df head_pos = player->getPosition();
	head_pos.Y+=69;
	p_cam->setPosition(head_pos);

	
	//p_cam->setPosition(player->getHeadBone()->getAbsolutePosition()+vector3df(0,4,0));
}


//create collision mesh from mesh polygons
NewtonCollision *CGameloop::createNCollisionMesh(NewtonWorld *world, IMesh *mesh)
{
	//create a newton mesh
	NewtonCollision * obj = ::NewtonCreateTreeCollision(world, NULL);
	::NewtonTreeCollisionBeginBuild(obj);

	int n_buffer = 0;
	int v_index[3] = {0,0,0};

	scene::IMeshBuffer *buffer;
	float array[9];

	for(n_buffer = 0; n_buffer < mesh->getMeshBufferCount(); n_buffer++)
	{
		buffer = mesh->getMeshBuffer(n_buffer);

		S3DVertex *vertices = (S3DVertex*)buffer->getVertices();
		u16 *indices = buffer->getIndices();

		for(int i=0; i<buffer->getIndexCount(); i+=3)
		{
			v_index[0] = indices[ i ];
			v_index[1] = indices[i+1];
			v_index[2] = indices[i+2];

			// 1st position vertex
			array[0] = vertices[ v_index[0] ].Pos.X;
			array[1] = vertices[ v_index[0] ].Pos.Y;
			array[2] = vertices[ v_index[0] ].Pos.Z;

			// 2nd position vertex
			array[3] = vertices[ v_index[1] ].Pos.X;
			array[4] = vertices[ v_index[1] ].Pos.Y;
			array[5] = vertices[ v_index[1] ].Pos.Z;

			// 3rd position vertex
			array[6] = vertices[ v_index[2] ].Pos.X;
			array[7] = vertices[ v_index[2] ].Pos.Y;
			array[8] = vertices[ v_index[2] ].Pos.Z;

			//Add new face to collision mesh
			NewtonTreeCollisionAddFace(obj, //collision mesh to add face to
				3, //number of vertices in array
				(float*)array, //pointer to vertex array
				3*sizeof(float),//size of each vertex
				1); //ID of the face
		}
	}

	::NewtonTreeCollisionEndBuild(obj,0);

	return obj;
}
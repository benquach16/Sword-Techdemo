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

#ifndef _PERSON_H_
#define _PERSON_H_

#pragma once

#include "irrlicht.h"
#include "irrklang.h"
#include "Newton.h"
#include "dMatrix.h"
#include "dVector.h"
#include "healthbar.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "vector"



using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace RakNet;


//legacy, delete later
#pragma pack(push, 1)
struct NetworkData
{
	unsigned char name;
	float x;
	float y;
	float z;
	float rotX;
	float rotY;
	float rotZ;
	bool slashLeft;
	bool slashRight;
	bool slashTop;
};
#pragma pack(pop)


struct SObject
{
	scene::IAnimatedMeshSceneNode *mesh;
	NewtonCollision *coll;
};

//state enum for finite state machine
enum AI_STATE
{
	STATE_RUN,
	STATE_FLEE,
	STATE_DEFEND,
	STATE_ATTACK,
};

//person class
class CPerson
{
public:
	scene::IAnimatedMeshSceneNode *model;
	scene::IAnimatedMeshSceneNode *sword;

	int swing_force;
	int timer;
	int MOVEMENTSPEED;

	//default constructor
	CPerson(irr::IrrlichtDevice *graphics = 0,
		NewtonWorld *world = 0,
		vector3df &pos = vector3df(0,0,0),
		vector3df &rot = vector3df(0,0,0),
		bool isplayer = false);
	~CPerson();

	//delete this object
	void drop()
	{
		delete this;
	}

	//function run in gamemanager to do basic stuff that the npc should do
	virtual void run(f32 frameDeltaTime);
	//update position of finger
	virtual void updateFingerPosition();
	//update position of head
	virtual void updateHeadPosition();
	//Newton Function
	//Generates a newtoncollision from the animated mesh
	NewtonCollision *generateCollMesh(scene::IAnimatedMesh *mesh);
	//Newton Function
	//Generates a bounding Box
	NewtonCollision *generateBoundingBox(IAnimatedMesh *mesh); 

	//Self Explanitory
	void setPosition(vector3df &pos);
	//Self Explanitory
	void setRotation(vector3df &rot);

	//Self Explanitory
	void moveForward(f32 frameDeltaTime);
	void moveBackward(f32 frameDeltaTime);
	void moveLeft(f32 frameDeltaTime);
	void moveRight(f32 frameDeltaTime);
	void moveLegs(f32 frameDeltaTime);

	//return functions
	//get the position/rotation
	vector3df getPosition();
	vector3df getRotation();
	//mostly self explanitory
	void setRHandPosition(vector3df &pos);	
	void setRHandRotation(vector3df &rot);
	void setRForearmRotation(vector3df &rot);
	//Set the rotation of the right Shoulder
	void setRShoulderRotation(vector3df &rot);		
	void setRFingerRotation(vector3df &rot);
	void setRThighRotation(vector3df &rot);
	void setLThighRotation(vector3df &rot);
	//Rotate the right arm from left to right or right to left
	void rotateRArmX(float x);
	//Rotate the right arm up and down or down to up
	void rotateRArmY(float y);
	//Return rotation functions
	vector3df getRShoulderRotation();
	vector3df getRForearmRotation();
	vector3df getRHandRotation();
	vector3df getRFingerRotation();
	vector3df getRThighRotation();
	vector3df getLThighRotation();
	

	//defunct
	//callbacks
	static void _cdecl PhysicsSetTransform(const NewtonBody *body,const dFloat* matrix, int threadIndex);	//legacy func	
	static void TorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex);

	void damage(int damage);
	int getHealth();
	void returnToLastGoodPosition();
	void forceBack(vector3df &rotation, int amount, f32 frameDeltaTime);
	void die();

	//ai functions
	//Returns the AI state that the npc is currently in
	AI_STATE getState();
	//Set the NPC to a different AI state
	void setState(AI_STATE state);
	//Rotate to face a position
	void facePoint(vector3df &point);
	//Defunc
	void readySword();
	
	//used to see if the person is a player
	bool getIsPlayer()
	{
		return isplayer_;
	}
	//ensure that only one attack is possible per swing
	bool getAttacked()
	{
		return attacked_already;
	}
	void setAttacked(bool attacked)
	{
		attacked_already = attacked;
	}

	void setVisible(bool visible)
	{
		model->setVisible(visible);
		sword->setVisible(visible);
	}
	bool getVisible()
	{
		return model->isVisible();
	}



	//combat functions
	//rotates the hand to a slashing form
	void resetRotations();
	//Stops the npc from further slashing
	void resetSlash();
	//Tells the NPC to slash in a random direction
	void randomSlash(f32 frameDeltaTime);
	//If sword is blocked by another sword, call this function
	void recoil();
	//Slash right
	void slashRight();
	//boolean functions
	void setSlashRight(bool right)
	{
		slashRight_ = right;
	}
	bool getSlashRight()
	{
		return slashRight_;
	}
	void slashLeft();
	void setSlashLeft(bool left)
	{
		slashLeft_ = left;
	}
	bool getSlashLeft()
	{
		return slashLeft_;
	}
	void slashTop();
	void setSlashTop(bool top)
	{
		slashTop_ = top;
	}
	bool getSlashTop()
	{
		return slashTop_;
	}
	int getSwingForce()
	{
		return swing_force;
	}
	void setSwingForce(int force)
	{
		swing_force = force;
	}
	void underBlock();
	bool getUnderBlock()
	{
		return underBlock_; 
	}
	void setUnderBlock(bool block)
	{
		underBlock_ = block;
	}
	int getDefenseTime()
	{
		return defend_timer;
	}
	void setDefenseTime(int time)
	{
		defend_timer = time;
	}
	int getAttackTime()
	{
		return attack_timer;
	}
	void setAttackTime(int time)
	{
		attack_timer = time;
	}
	int getStamina()
	{
		return stamina;
	}
	void setStamina(int stamina)
	{
		this->stamina = stamina;
	}
	int getStaminaTime()
	{
		return stamina_timer;
	}
	void setStaminaTime(int stamina)
	{
		stamina_timer = stamina;
	}

	scene::IBoneSceneNode *getHeadBone();
	//return functions for newton
	SObject *getSObjectMesh()
	{
		return mesh_;
	}
	SObject *getSObjectSword()
	{
		return sword_;
	}
	NewtonBody *getMeshNewton()
	{
		return model_body_;
	}


private:
	//irrlicht
	irr::IrrlichtDevice *graphics;
	NewtonWorld *world;
	scene::ISceneNode *placer;
	CHealthBar *healthbar_;
	bool isplayer_;

	//health variable
	int health;
	//stamina variable
	int stamina;
	bool staggered;
	int staggered_timer;
	int stamina_timer;
	int force_timer;
	int attack_timer;
	int defend_timer;
	bool sword_attacked_already;
	bool attacked_already;	//this is necessary because collision updates itself every frame. if the enemy keeps getting attacked every frame, then he will suffer ridiculous amounts of damage and lag
	std::vector<int> timers;

	vector3df last_position;

	vector3df old_r_rot_;

	bool slashLeft_;
	bool slashRight_;
	bool slashTop_;
	bool underBlock_;

	bool left_leg;
	bool right_leg;

	AI_STATE state_;

	struct boneInfo
	{
		vector3df r_shoulder;
		vector3df r_forearm;
		vector3df r_hand;
	};
	boneInfo initial_bone;

	SObject *sword_;
	SObject *mesh_;

	NewtonBody *sword_body_;
	NewtonBody *model_body_;
};

#endif
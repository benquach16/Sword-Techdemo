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
#include "player.h"

int MOVEMENTSPEED = 9;

CPlayer::CPlayer(irr::IrrlichtDevice *graphics, NewtonWorld *world, vector3df &pos, vector3df &rot, const char *name) :
CPerson(graphics,world,pos,rot,true), graphics(graphics), name_(name)
{
	swing_force = 80;
	size_t size = strlen(name)+1;
	wchar_t *wc = new wchar_t[size];
	mbstowcs(wc,name,size);
	player_name = graphics->getGUIEnvironment()->addStaticText(wc,rect<s32>(0,0,200,30),false,true);
	player_name->setOverrideColor(SColor(255,255,255,0));
	/*
	vector3df _rot = this->getRShoulderRotation();
	_rot.X -= 50;
	_rot.Y = 260;
	_rot.Z += 180;
	this->setRShoulderRotation(_rot);
	*/
	//model->getJointNode("Bip001RFinger")->addChild(sword);
}

CPlayer::~CPlayer()
{
}

void CPlayer::run(f32 frameDeltaTime)
{

	model->getJointNode("Bip001RFinger")->updateAbsolutePosition();
	sword->setPosition(model->getJointNode("Bip001RFinger")->getAbsolutePosition());
	sword->setRotation(model->getJointNode("Bip001RFinger")->getAbsoluteTransformation().getRotationDegrees());

	//make bones conform to a proper circle
	vector3df rot = this->getRShoulderRotation();
	
	if(rot.Y > 360)
		rot.Y -= 360;
	if(rot.Y < 0)
		rot.Y +=360;
	if(rot.Z > 360)
		rot.Z -= 360;
	if(rot.Z < 0)
		rot.Z +=360;
	this->setRShoulderRotation(rot);

	vector3df f_rot = this->getRForearmRotation();
	if(f_rot.Y > 360)
		f_rot.Y -= 360;
	if(f_rot.Y < 0)
		f_rot.Y +=360;
	if(f_rot.Z > 360)
		f_rot.Z -= 360;
	if(f_rot.Z < 0)
		f_rot.Z +=360;
	this->setRForearmRotation(f_rot);
	/*
	vector3df f_rot_ = this->getRFingerRotation();
	if(f_rot_.Y > 360)
		f_rot_.Y -= 360;
	if(f_rot_.Y < 0)
		f_rot_.Y +=360;
	if(f_rot_.Z > 360)
		f_rot_.Z -= 360;
	if(f_rot_.Z < 0)
		f_rot_.Z +=360;
	this->setRFingerRotation(f_rot);
	*/
	if(timer < graphics->getTimer()->getTime())
	{
		swing_force = 10;
		timer = graphics->getTimer()->getTime()+500;
	}
	updateFingerPosition();
	updateHeadPosition();

	model->getJointNode("Bip001")->updateAbsolutePositionOfAllChildren();
	vector3df tmp = getPosition();
	tmp.Y += 80;
	vector2d<s32> name_pos = graphics->getSceneManager()->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(tmp,graphics->getSceneManager()->getActiveCamera());
	name_pos.X-=50;
	player_name->setRelativePosition(name_pos);

	if(getHealth() < 1)
	{
		deathScreen();
	}
}



void CPlayer::rotateFacingX(float x)
{
	
	vector3df rot = getHeadRotation();
	rot.X +=x;
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001Spine1");
	vector3df _rot = bone->getRotation();
	_rot.Z+=x;
	//bone->setRotation(_rot);
	
	//this->setRotation(rot);
	//this->getHeadBone()->setRotation(rot);
	
	if(rot.X > 40)
	{
		rot.X = 40;
	}
	if(rot.X < -60)
	{
		rot.X = -60;
	}
	
	head_rotation = rot;
	
}

void CPlayer::rotateFacingY(float y)
{
	vector3df rot = getRotation();

	rot.Y += y;
	swing_force+=abs(y);
	this->setRotation(rot);
}


vector3df CPlayer::getHeadRotation()
{
	return head_rotation;
}

void CPlayer::setHeadRotation(vector3df rot)
{
	head_rotation = rot;

}

void CPlayer::deathScreen()
{
	//show red stuff
	//to indicate that you died.
	vector2d<u32> size = graphics->getVideoDriver()->getScreenSize();
	graphics->getVideoDriver()->draw2DRectangle(rect<s32>(0,0,size.X,size.Y),SColor(255,255,0,0),SColor(255,255,0,0),
		SColor(255,255,0,0),SColor(255,255,0,0));
}

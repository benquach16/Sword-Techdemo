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
#include "person.h"

//temp constant
const int SLASH_TIME = 2000;

#define MAX_STAMINA 100

CPerson::CPerson(irr::IrrlichtDevice *graphics, NewtonWorld *world, vector3df &pos, vector3df &rot, bool isplayer): 
graphics(graphics), world(world), 
	slashRight_(false), slashLeft_(false), slashTop_(false), underBlock_(false),
	attacked_already(false),
	isplayer_(isplayer), state_(STATE_RUN), left_leg(false), right_leg(false)
{
	//initializer
	swing_force		=	80;
	state_			=	STATE_RUN;

	stamina_timer	=	graphics->getTimer()->getTime();
	timer			=	graphics->getTimer()->getTime()+1000;
	attack_timer	=	graphics->getTimer()->getTime()+2000;
	defend_timer	=	graphics->getTimer()->getTime()+2000;

	MOVEMENTSPEED	=	20;
	health			=	100;
	stamina			=	100;
	//create character
	model = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/man.x"));
	//model->setScale(vector3df(0.3,0.3,0.3));

	model->setJointMode(scene::EJUOR_CONTROL);
	//model->setTransitionTime(0.1f);
	model->setAnimationSpeed(30);
	model->setPosition(pos);
	model->setRotation(rot);
	//model->animateJoints();
	model->addShadowVolumeSceneNode();
	model->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);
	sword = graphics->getSceneManager()->addAnimatedMeshSceneNode(graphics->getSceneManager()->getMesh("res/models/sword.x"));
	model->getMaterial(0).SpecularColor.set(0,0,0,0);

	healthbar_ = new CHealthBar(graphics,model);

	//reset arm orientation
	this->rotateRArmX(1);
	this->rotateRArmY(1);

	//create newton collision objects
	sword_ = new SObject;
	sword_->mesh = sword;
	sword_->coll = generateBoundingBox(sword->getMesh());

	mesh_ = new SObject;
	mesh_->mesh = model;
	mesh_->coll = generateCollMesh(model->getMesh());

	sword_body_ = NewtonCreateBody(world, sword_->coll, (float*)&sword->getAbsoluteTransformation());
	model_body_ = NewtonCreateBody(world, mesh_->coll, (float*)&model->getAbsoluteTransformation());
	NewtonBodySetUserData(sword_body_, sword);
	NewtonBodySetUserData(model_body_, model);
	//NewtonBodySetMassMatrix(model_body_,10,150,150,150);


	NewtonBodySetTransformCallback(model_body_, PhysicsSetTransform);
	NewtonBodySetForceAndTorqueCallback(model_body_,TorqueCallback);

	float omega[3]; omega[0] = 1.0f; omega[1] = 0.0f; omega[2] = 0.0f;
	NewtonBodySetOmega (model_body_, &omega[0]);

	CMatrix4<float> mat;

	mat.setTranslation(getPosition());
	NewtonBodySetMatrix(model_body_, mat.pointer());

	float pPoint[3];
	pPoint[0] = model->getJointNode("Bip001RFinger")->getAbsolutePosition().X;
	pPoint[1] = model->getJointNode("Bip001RFinger")->getAbsolutePosition().Y;
	pPoint[2] = model->getJointNode("Bip001RFinger")->getAbsolutePosition().Z;
	::NewtonConstraintCreateBall(world,pPoint,sword_body_,model_body_);
	vector3df _rot = this->getRShoulderRotation();
	_rot.X -= 50;
	_rot.Y -= 50;
	_rot.Z += 180;
	this->setRShoulderRotation(_rot);

	last_position = getPosition();

}

CPerson::~CPerson()
{
}

void CPerson::die()
{
	//stop swinging and stuff
	//roll over and die
	vector3df rot = model->getRotation();
	if(rot.X > -90)
	{
		rot.X -= 1;
		model->setRotation(rot);
	}
}

void CPerson::run(f32 frameDeltaTime)
{
	
	//stuff goes here
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
	if(rot.Z > 320)
		rot.Z = 320;
	if(rot.Z < 220)
		rot.Z = 220;
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

	

	if(health >0)
	{
		//including timers in one vector may help
		if(timer < graphics->getTimer()->getTime())
		{
			swing_force=20;
			timer = graphics->getTimer()->getTime()+1000;

		}
		if(getStaminaTime() < graphics->getTimer()->getTime())
		{
			if(getStamina() < 100)
			{
				setStamina(getStamina()+1);
				setStaminaTime(graphics->getTimer()->getTime()+50);
			}
		}

		if(getState()==STATE_RUN)
		{
			moveForward(frameDeltaTime);
			resetSlash();
		}
		//if the AI is hurt
		if(getState()==STATE_FLEE)
		{
			moveBackward(frameDeltaTime);
		}
		//if the AI wants to attack
		if(getState()==STATE_ATTACK)
		{
			if(attack_timer < graphics->getTimer()->getTime())
			{
				//if the attack time is up
				//then change to a defensive stance to block incoming blows
				
				setState(STATE_DEFEND);
				attack_timer = graphics->getTimer()->getTime()+ 2000;
				defend_timer = graphics->getTimer()->getTime()+2000;
			}
			if(getSlashLeft())
			{
				float turnrate = 3*getSwingForce() * frameDeltaTime;
				rotateRArmX(turnrate);
				if(getRShoulderRotation().Z > 310)
				{
					setSlashLeft(false);
				}
			}
			if(getSlashRight())
			{

				float turnrate = -3 *getSwingForce() * frameDeltaTime;
				rotateRArmX(turnrate);
				if(getRShoulderRotation().Z < 250)
				{
					setSlashRight(false);
				}
			}
			if(getSlashTop())
			{
				float turnrate = 2*getSwingForce() * frameDeltaTime;
				rotateRArmY(turnrate);

			}
		}

		//if the AI wants to defend
		if(getState()==STATE_DEFEND)
		{
			if(defend_timer < graphics->getTimer()->getTime())
			{
				setState(STATE_ATTACK);
				defend_timer = graphics->getTimer()->getTime()+2000;
				attack_timer = graphics->getTimer()->getTime()+2000;
			}
		}
	}
	else
	{
		die();
	}
	//draw healthbar
	healthbar_->run(health);

	//needed to ensure that sword remains in proper position
	updateFingerPosition();
	updateHeadPosition();

	//need an efficient method
	//mesh_->coll = this->generateCollMesh(model->getMesh());

}

void CPerson::updateFingerPosition()
{
	//update position of finger so sword isnt fucked
	scene::IBoneSceneNode *Bone = model->getJointNode("Bip001RFinger");
	ISceneNode* tempNode = Bone;
	std::vector<ISceneNode*> Parents;

	while(model != tempNode)
	{
		tempNode = tempNode->getParent();
		Parents.push_back(tempNode);
	}

	for(int i = Parents.size() - 1; i >= 0; i--)
		Parents[i]->updateAbsolutePosition();

	Parents.clear();
}

//mostly to keep bones in check
void CPerson::updateHeadPosition()
{
	scene::IBoneSceneNode *Bone = model->getJointNode("Bip001Head");
	ISceneNode* tempNode = Bone;
	std::vector<ISceneNode*> Parents;

	while(model != tempNode)
	{
		tempNode = tempNode->getParent();
		Parents.push_back(tempNode);
	}

	for(int i = Parents.size() - 1; i >= 0; i--)
		Parents[i]->updateAbsolutePosition();

	Parents.clear();
}

void CPerson::returnToLastGoodPosition()
{
	setPosition(last_position);
}

void CPerson::forceBack(vector3df &rotation, int amount, f32 frameDeltaTime)
{
	vector3df pos = getPosition();
	float i = rotation.Y;

	pos.X=frameDeltaTime*amount*(sin(i * 3.14/180));
	pos.X+=getPosition().X;

	pos.Z=frameDeltaTime*amount*(cos(i * 3.14/180));
	pos.Z+=getPosition().Z;
	setPosition(pos);
}

void CPerson::moveLegs(f32 frameDeltaTime)
{
	//programmer art leg movement
	//what the fuck have i done
	if(left_leg == false)
	{
		vector3df rot_l = this->getLThighRotation();
		if(rot_l.Z > 130)
			rot_l.Z -= 20 * frameDeltaTime;
		else
			left_leg = true;
		this->setLThighRotation(rot_l);
	}

	vector3df rot_r = this->getRThighRotation();
	if(rot_r.Z < -80)
		rot_r.Z += 20 * frameDeltaTime;
	this->setRThighRotation(rot_r);
}

void CPerson::moveForward(f32 frameDeltaTime)
{
	float y = getRotation().Y;

	vector3df pPos;
	pPos = this->getPosition();
	pPos.X+=MOVEMENTSPEED *frameDeltaTime *(sin(y * 3.14/180));
	pPos.Z+=MOVEMENTSPEED *frameDeltaTime *(cos(y*3.14/180));

	this->setPosition(pPos);
	model->setLoopMode(true);
	model->setFrameLoop(10,20);
	
	//moveLegs(frameDeltaTime);
	
}


void CPerson::moveBackward(f32 frameDeltaTime)
{
	float x = getRotation().X;
	float y = getRotation().Y;

	vector3df pPos;
	pPos = getPosition();
	pPos.X-=MOVEMENTSPEED *frameDeltaTime *(sin(y * 3.14/180));
	pPos.Z-=MOVEMENTSPEED *frameDeltaTime *(cos(y*3.14/180));

	this->setPosition(pPos);
}

void CPerson::moveLeft(f32 frameDeltaTime)
{
	float x = getRotation().X;
	float y = getRotation().Y;
	y-=90;
	vector3df pPos;
	pPos = getPosition();
	pPos.X+=MOVEMENTSPEED *frameDeltaTime *(sin(y * 3.14/180));
	pPos.Z+=MOVEMENTSPEED *frameDeltaTime *(cos(y*3.14/180));

	this->setPosition(pPos);
}

void CPerson::moveRight(f32 frameDeltaTime)
{
	float x = getRotation().X;
	float y = getRotation().Y;
	y+=90;
	vector3df pPos;
	pPos = getPosition();
	pPos.X+=MOVEMENTSPEED *frameDeltaTime *(sin(y * 3.14/180));
	pPos.Z+=MOVEMENTSPEED *frameDeltaTime *(cos(y*3.14/180));

	this->setPosition(pPos);
}

void CPerson::rotateRArmX(float x)
{
	if(health > 0)
	{
		vector3df rot = this->getRShoulderRotation();
		//assign constraints
		int t = MAX_STAMINA / getStamina();
		swing_force+=abs(x/5);
		if(getIsPlayer())
			x = x/t;
		if(getSlashLeft() || getSlashRight() || getSlashTop())
		{
			if(stamina > 1)
				stamina--;
		}
		if(x >0)
		{
			if(getSlashLeft())
			{
				rot.Z += x;
			}
			else if (getSlashRight())
			{
				
				rot.Z += x/4;
			}
			else
				rot.Z += x;
		}
		else
		{
			if(getSlashRight())
			{
				rot.Z += x;
			}
			else if (getSlashLeft())
			{
				rot.Z += x/4;
			}
			else
				rot.Z += x;
		}

		if(rot.Z > 330)
			rot.Z = 330;
		if(rot.Z < 230)
			rot.Z = 230;

		//this->setRShoulderRotation(rot);
		this->setRShoulderRotation(rot);
	}
}

void CPerson::rotateRArmY(float y)
{
	if(health > 0)
	{
		swing_force+=abs(y/5);
		vector3df rot = this->getRShoulderRotation();


		rot.Y +=y;

		//arm goes thru 40 - 300
		if(rot.Y > 40)
		{
			if(rot.Y < 180)
				rot.Y = 40;
			else if(rot.Y <300)
				rot.Y = 300;
		}

		//this->setRShoulderRotation(rot);
		this->setRShoulderRotation(rot);
	}
}

void CPerson::recoil()
{
	//knock sword back

	if(getSlashLeft())
	{
		this->setRShoulderRotation(getRShoulderRotation() + vector3df(0,0,-50));
		setSlashLeft(false);
	}
	if(getSlashRight())
	{
		this->setRShoulderRotation(getRShoulderRotation() + vector3df(0,0,50));
		setSlashRight(false);
	}
	if(getSlashTop())
	{
		this->setRShoulderRotation(getRShoulderRotation() + vector3df(0,-50,0));
		setSlashRight(false);
	}

	swing_force = -20;
}

void CPerson::resetSlash()
{
	setSlashLeft(false);
	setSlashRight(false);
	setSlashTop(false);
}

//basic functions
void CPerson::setPosition(vector3df &pos)
{
	model->setPosition(pos);
}

void CPerson::setRotation(vector3df &rot)
{
	model->setRotation(rot);
}

void CPerson::setRHandPosition(vector3df &pos)
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RHand");
	bone->setPosition(bone->getPosition() + pos);
}

void CPerson::setRHandRotation(vector3df &rot)
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RHand");
	bone->setRotation(rot);
	bone->updateAbsolutePositionOfAllChildren();
}

void CPerson::setRForearmRotation(vector3df &rot)
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RForearm");
	bone->setRotation(rot);
	//implement inverse kinematics
}

void CPerson::setRShoulderRotation(vector3df &rot)
{
	//model->animateJoints();
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RUpperArm");
	bone->setRotation(rot);
	bone->updateAbsolutePositionOfAllChildren();
}

void CPerson::setRFingerRotation(vector3df &rot)
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RFinger");
	bone->setRotation(rot);
	bone->updateAbsolutePositionOfAllChildren();
}

void CPerson::setRThighRotation(vector3df &rot)
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RThigh");
	bone->setRotation(rot);
	bone->updateAbsolutePositionOfAllChildren();
}

void CPerson::setLThighRotation(vector3df &rot)
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001LThigh");
	bone->setRotation(rot);
	bone->updateAbsolutePositionOfAllChildren();
}

vector3df CPerson::getRShoulderRotation()
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RUpperArm");
	return bone->getRotation();
}
//returns rotation of forearm bone
vector3df CPerson::getRForearmRotation()
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RForearm");
	return bone->getRotation();
}

vector3df CPerson::getRHandRotation()
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RHand");
	return bone->getRotation();
}

vector3df CPerson::getRFingerRotation()
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RFinger");
	return bone->getRotation();
}

vector3df CPerson::getRThighRotation()
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RThigh");
	return bone->getRotation();
}

vector3df CPerson::getLThighRotation()
{
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001LThigh");
	return bone->getRotation();
}

void CPerson::resetRotations()
{
	this->setRFingerRotation(vector3df(0,0,0));
	this->setRForearmRotation(vector3df(0,0,0));
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RUpperArm");
	if(this->getRShoulderRotation().X < 310)
	{
		bone->setRotation(bone->getRotation()+vector3df(1,0,0));
	}
}

void CPerson::slashRight()
{
	//programmer art goes here
	//this function meant to be looped
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RUpperArm");
	scene::IBoneSceneNode *forearm = model->getJointNode("Bip001RForearm");
	scene::IBoneSceneNode *hand = model->getJointNode("Bip001RHand");
	scene::IBoneSceneNode *finger = model->getJointNode("Bip001RFinger");
	
	this->setRFingerRotation(vector3df(0,-1.5*getRShoulderRotation().Z-70,0));
	this->setRForearmRotation(vector3df(70,-1*getRShoulderRotation().Z-90,0));
	if(bone->getRotation().X > 270)
	{
		bone->setRotation(bone->getRotation()+vector3df(-1,0,0));
	}
	if(finger->getRotation().Y > -90)
	{
		//finger->setRotation(finger->getRotation()+vector3df(0,-5,0));
	}

}

void CPerson::slashLeft()
{
	//programmer art goes here
	//this function meant to be looped
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RUpperArm");
	scene::IBoneSceneNode *forearm = model->getJointNode("Bip001RForearm");
	scene::IBoneSceneNode *hand = model->getJointNode("Bip001RHand");
	scene::IBoneSceneNode *finger = model->getJointNode("Bip001RFinger");

	this->setRFingerRotation(vector3df(0,-1.5*getRShoulderRotation().Z+30,0));
	this->setRForearmRotation(vector3df(70,-getRShoulderRotation().Z-90,0));
	if(bone->getRotation().X < 90)
	{
		bone->setRotation(bone->getRotation()+vector3df(1,0,0));
	}
}

void CPerson::slashTop()
{
	//programmer art goes here
	//this function meant to be looped
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RUpperArm");
	scene::IBoneSceneNode *forearm = model->getJointNode("Bip001RForearm");
	scene::IBoneSceneNode *hand = model->getJointNode("Bip001RHand");
	scene::IBoneSceneNode *finger = model->getJointNode("Bip001RFinger");
	this->setRFingerRotation(vector3df(0,270,0));
	this->setRForearmRotation(vector3df(0,0,2*getRShoulderRotation().Y-30));
	if(bone->getRotation().X > 290)
	{
		bone->setRotation(bone->getRotation()+vector3df(-1,0,0));
	}
}

void CPerson::underBlock()
{
	//cool stuff goes here
	scene::IBoneSceneNode *bone = model->getJointNode("Bip001RUpperArm");
	scene::IBoneSceneNode *forearm = model->getJointNode("Bip001RForearm");
	scene::IBoneSceneNode *hand = model->getJointNode("Bip001RHand");
	scene::IBoneSceneNode *finger = model->getJointNode("Bip001RFinger");
	this->setRFingerRotation(vector3df(90,0,0));
	this->setRForearmRotation(vector3df(70,30,0));
}

void CPerson::setState(AI_STATE state)
{
	state_ = state;
}

AI_STATE CPerson::getState()
{
	return state_;
}

void CPerson::randomSlash(f32 frameDeltaTime)
{

	int i = rand()%3+1;
	switch(i)
	{
	case 1: 
		{
			float turnrate = getSwingForce() * frameDeltaTime;
			rotateRArmX(turnrate);
			setSlashLeft(true);
			break;
		}
	case 2:
		{
			float turnrate = getSwingForce() * frameDeltaTime;
			rotateRArmX(turnrate);
			setSlashRight(true);
			break;
		}
	case 3:
		{
			float turnrate = -getSwingForce() * frameDeltaTime;
			rotateRArmY(turnrate);
			setSlashLeft(true);
			break;
		}
	}
}

void CPerson::damage(int damage)
{
	if(health > 0)
	{
		health-= damage;
	}
}

//return health
int CPerson::getHealth()
{
	return health;
}


//return vectors
vector3df CPerson::getPosition()
{
	return model->getPosition();
}

vector3df CPerson::getRotation()
{
	return model->getRotation();
}

//return bones
scene::IBoneSceneNode *CPerson::getHeadBone()
{
	IBoneSceneNode *bone = model->getJointNode("Bip001Head");
	return bone;
}

//ai functions
void CPerson::facePoint(vector3df &point)
{
	vector3df pos = point - getPosition();
	vector3df diff = pos.getHorizontalAngle();
	setRotation(diff);
}

void CPerson::readySword()
{
	this->setRShoulderRotation(vector3df(260,50,90));
}

NewtonCollision *CPerson::generateCollMesh(IAnimatedMesh *mesh)
{
	u32 nVertices = 0, nMeshBuffer;
	for( nMeshBuffer = 0 ; nMeshBuffer < mesh->getMeshBufferCount(); ++nMeshBuffer)
	{
		scene::IMeshBuffer *buffer = mesh->getMeshBuffer(nMeshBuffer);
		nVertices += buffer->getVertexCount();
	}

	// allocate block for positions of every vertex in mesh, no need to delete
	// anything, the array cleans up for us.
	core::array<core::vector3df> vertices;
	vertices.reallocate(nVertices);

	//Get mesh buffers and copy face vertices
	for( nMeshBuffer = 0 ; nMeshBuffer < mesh->getMeshBufferCount(); ++nMeshBuffer)
	{
		scene::IMeshBuffer *buffer = mesh->getMeshBuffer(nMeshBuffer);

		// handle the irrlicht supported vertex types
		switch(buffer->getVertexType())
		{
		case video::EVT_STANDARD:
			{
				video::S3DVertex* verts = (video::S3DVertex*)buffer->getVertices();
				for(u32 v = 0; v < buffer->getVertexCount(); ++v)
					vertices.push_back(verts[v].Pos);
			}
			break;

		case video::EVT_2TCOORDS:
			{
				video::S3DVertex2TCoords* verts = (video::S3DVertex2TCoords*)buffer->getVertices();
				for(u32 v = 0; v < buffer->getVertexCount(); ++v)
					vertices.push_back(verts[v].Pos);
			}
			break;

		case video::EVT_TANGENTS:
			{
				video::S3DVertexTangents* verts = (video::S3DVertexTangents*)buffer->getVertices();
				for(u32 v = 0; v < buffer->getVertexCount(); ++v)
					vertices.push_back(verts[v].Pos);
			}
			break;

		default:
			return 0; // don't know vertex type! bail.
		}
	}

	//Create Newton collision object
	return NewtonCreateConvexHull(world, nVertices, &vertices[0].X, sizeof(core::vector3df), NULL,0,NULL);
}

void _cdecl CPerson::PhysicsSetTransform(const NewtonBody *body,const dFloat* matrix, int threadIndex)
{
	ISceneNode* node = (ISceneNode*) NewtonBodyGetUserData(body);
	matrix4 transform;
	transform.setM(matrix);

	node->setPosition(transform.getTranslation());
	node->setRotation(transform.getRotationDegrees());
}

void CPerson::TorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex) 
{
   float mass; 
   float Ixx; 
   float Iyy; 
   float Izz; 
   float force[3]; 
   float torque[3]; 

   NewtonBodyGetMassMatrix (body, &mass, &Ixx, &Iyy, &Izz); 

   force[0] = 0.0f; 
   force[1] = -9.8 * mass; 
   force[2] = 0.0f; 

   torque[0] = 0.0f; 
   torque[1] = 0.0f; 
   torque[2] = 0.0f; 

   NewtonBodyAddForce (body, force); 
   NewtonBodyAddTorque (body, torque);
}

NewtonCollision* CPerson::generateBoundingBox(IAnimatedMesh *mesh)
{
{

    //calculate the dimensions of the bounding box
	vector3df boundingBoxSize(mesh->getBoundingBox().MaxEdge.X - mesh->getBoundingBox().MinEdge.X,
                              mesh->getBoundingBox().MaxEdge.Y - mesh->getBoundingBox().MinEdge.Y,
                              mesh->getBoundingBox().MaxEdge.Z - mesh->getBoundingBox().MinEdge.Z);


    //get the relative center of the bounding box and put it to a matrix4
    matrix4 boundingBoxCenterMatrix;
    boundingBoxCenterMatrix.setTranslation(mesh->getBoundingBox().getCenter());

    /* make the newton collision box from the dimensions of the bounding box
     and offset it to the center of the bounding box */
    return NewtonCreateBox(world, 
                           boundingBoxSize.X, 
                           boundingBoxSize.Y, 
                           boundingBoxSize.Z, 
						   0,
                           boundingBoxCenterMatrix.pointer());
}
}
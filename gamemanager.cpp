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
#include "gamemanager.h"

CGameManager::CGameManager(irr::IrrlichtDevice *graphics, irrklang::ISoundEngine *sound, NewtonWorld *world) : graphics(graphics), world(world), sound(sound)
{
}

void CGameManager::run(f32 frameDeltaTime)
{
	runEffects();
	for(unsigned int i=0; i< person_manager.size(); i++)
	{
		//go thru each
		CPerson *person = person_manager[i];
		vector3df pos = person->getPosition();
		person->run(frameDeltaTime);
		if(person->getHealth() > 0)
		{
			if(person->getSlashRight())
			{
				person->slashRight();
			}
			else
			{
				if(person->getSlashLeft())
				{
					person->slashLeft();
				}

				else
				{
					if(person->getSlashTop())
					{

						person->slashTop();
					}
					else
					{
						//reset bones
						person->resetRotations();
					}
				}
			}

			for(int n=0; n< person_manager.size(); n++)
			{
				if(person != person_manager[n])
				{
					//ensure that they are not the same

					if(checkCollision(person->getSObjectMesh(), person_manager[n]->getSObjectMesh()))
					{
						//person to person collision
						//adjust so that they dont go into each other
						//push em back a bit

						vector3df pos = person_manager[n]->getPosition() - person->getPosition();
						vector3df rot = -(pos.getHorizontalAngle());
						person->forceBack(rot,5,frameDeltaTime);

					}

					if(checkCollision(person->getSObjectSword(), person_manager[n]->getSObjectSword()))
					{
						//BLOCKED NIGGA
						if(person->getSlashLeft() || person->getSlashRight() || person->getSlashTop())
						{
							person->recoil();

							person->moveBackward(frameDeltaTime);
							vector3df point = checkCollisionPoint(person->getSObjectSword(), person_manager[n]->getSObjectSword());
							sound->play3D("res/sounds/clash.wav",point,false,false,false);
							CEffect *s = new CSpark(graphics,sound,point);
							addEffect(s);
						}
					}

					if(checkCollision(person->getSObjectSword(), person_manager[n]->getSObjectMesh()))
					{
						//sword sliced man
						//make sure that he is slashing first
						if(person->getSlashLeft() || person->getSlashRight() || person->getSlashTop())
						{
							if(person_manager[n]->getAttacked()!=true)
							{
								person_manager[n]->damage(person->getSwingForce()/2);
								vector3df point = checkCollisionPoint(person->getSObjectSword(), person_manager[n]->getSObjectMesh());
								sound->play3D("res/sounds/cut.wav",point,false,false,false);
								CEffect *b = new CBlood(graphics,sound,point);
								addEffect(b);
								person_manager[n]->setAttacked(true);
							}
						}
					}
					else
					{
						person_manager[n]->setAttacked(false);
					}

					//now that collision is done
					//go on to ai functions
					if(person->getIsPlayer()==false)
					{
						if(person->getHealth() > 0)
						{

							//ensure player doesnt get controlled by computer
							person->facePoint(person_manager[n]->getPosition());

							//person->readySword();
							if(pos.getDistanceFrom(person_manager[n]->getPosition()) > 200)
							{
								person->setState(STATE_RUN);
							}
							if(pos.getDistanceFrom(person_manager[n]->getPosition()) < 90)
							{
								if(person->getState()==STATE_RUN)
									person->setState(STATE_ATTACK);
							}
							if(person->getState() == STATE_ATTACK)
							{
								if(person->getSlashLeft()==false && person->getSlashRight() ==false && person->getSlashTop()==false)
								{

									person->randomSlash(frameDeltaTime);
								}
								if(pos.getDistanceFrom(person_manager[n]->getPosition()) > 90)
								{
									person->setState(STATE_RUN);
								}
								vector3df rot = person->getRShoulderRotation();
								rot.Y = person_manager[n]->getRShoulderRotation().Y +20;
								person->setRShoulderRotation(rot);
							}
							if(person->getState() == STATE_DEFEND)
							{
								person->resetSlash();
								if(person_manager[n]->getSlashLeft())
								{
									float turnrate = 90* frameDeltaTime;
									person->rotateRArmX(turnrate);
									vector3df rot = person->getRShoulderRotation();
									rot.Y = person_manager[n]->getRShoulderRotation().Y +20;
									person->setRShoulderRotation(rot);
								}
								if(person_manager[n]->getSlashRight())
								{
									float turnrate = -90 * frameDeltaTime;
									person->rotateRArmX(turnrate);
									vector3df rot = person->getRShoulderRotation();
									rot.Y = person_manager[n]->getRShoulderRotation().Y +20;
									person->setRShoulderRotation(rot);
								}
								if(person_manager[n]->getSlashTop())
								{
									float turnrate = -90 * frameDeltaTime;
									person->rotateRArmY(turnrate);
									person->rotateRArmX(-turnrate);
								}
							}
						}

					}
				}
			}

		}
	}


}

void CGameManager::runEffects()
{
	for(unsigned int i=0; i< effect_manager.size(); i++)
	{
		effect_manager[i]->run();
		if(effect_manager[i]->getEnd())
		{
			//effect finished delete
			effect_manager[i]->drop();
			effect_manager.erase(effect_manager.begin()+i);
		}
	}
}

bool CGameManager::checkCollision(SObject *a, SObject *b)
{
	matrix4 matA;
	matrix4 matB;

	matA.makeIdentity();
	matB.makeIdentity();

	matA.setTranslation(a->mesh->getAbsolutePosition());
	matA.setRotationDegrees(a->mesh->getRotation());
	matB.setTranslation(b->mesh->getAbsolutePosition());
	matB.setRotationDegrees(b->mesh->getRotation());

	const int n_contacts = 2;

	float contacts[n_contacts * 3];
	float normals [n_contacts * 3];
	float penetration [n_contacts];
	int hits = NewtonCollisionCollide(world, n_contacts, a->coll, (float*)&matA, b->coll, (float*)&matB,contacts,normals,penetration,0);

	if(hits > 0)
		return true;
	return false;

}

vector3df CGameManager::checkCollisionPoint(SObject *a, SObject *b)
{
	matrix4 matA;
	matrix4 matB;

	matA.makeIdentity();
	matB.makeIdentity();

	matA.setTranslation(a->mesh->getAbsolutePosition());
	matA.setRotationDegrees(a->mesh->getRotation());
	matB.setTranslation(b->mesh->getAbsolutePosition());
	matB.setRotationDegrees(b->mesh->getRotation());

	const int n_contacts = 2;

	float contacts[n_contacts * 3];
	float normals [n_contacts * 3];
	float penetration [n_contacts];
	int hits = NewtonCollisionCollide(world, n_contacts, a->coll, (float*)&matA, b->coll, (float*)&matB,contacts,normals,penetration,0);

	vector3df tmp;
	tmp.X = contacts[3];
	tmp.Y = contacts[4];
	tmp.Z = contacts[5];

	return tmp;
}

void CGameManager::addPerson(CPerson *person)
{
	person_manager.push_back(person);
}

void CGameManager::addEffect(CEffect *effect)
{
	effect_manager.push_back(effect);
}
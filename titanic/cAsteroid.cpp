/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cAsteroid.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cAsteroid::cAsteroid() : cSprite()
{
	this->asteroidVelocity = { 0.0, 0.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cAsteroid::update(double deltaTime)
{

	
	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x -= this->getSpriteTranslation().x * deltaTime +5;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;

	if (currentSpritePos.x < -100)
	{
		currentSpritePos.x = 1200; 
	}

	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	cout << "Asteroid position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the Asteroid
=================================================================
*/
void cAsteroid::setAsteroidVelocity(SDL_Point AsteroidVel)
{
	asteroidVelocity = AsteroidVel;
}
/*
=================================================================
  Gets the Asteroid velocity
=================================================================
*/
SDL_Point cAsteroid::getAsteroidVelocity()
{
	return asteroidVelocity;
}
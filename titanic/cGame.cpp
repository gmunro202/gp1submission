/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();

int score = 0;


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	// Store the textures
	textureName = { "asteroid1", "bullet","theRocket","theBackground"};
	texturesToUse = { "Images\\ice.png",  "Images\\fireball.png", "Images\\Titanic.png", "Images\\sea1024x768.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	

	btnNameList = { "exit_btn", "start", "retry"};
	btnTexturesToUse = { "Images/quit.png", "Images/start.png", "Images/Buttons/retry.png" };


	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	player.push_back(new cRocket);
	player[0]->setSpritePos({ 100, 350 });
	player[0]->setTexture(theTextureMgr->getTexture("theRocket"));
	player[0]->setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
	player[0]->setRocketVelocity({ 0, 0 });

	// Create vector array of textures
	srand(time(NULL));
	for (int astro = 0; astro < 5; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		int xPosition = 100 * (rand() % 5 + 1) + 500;
		int yPosition = 100 * (rand() % 5 + 1); 
		theAsteroids[astro]->setSpritePos({2 ,  (rand() % 700)});
		theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
		int randAsteroid = rand() % 4;
		theAsteroids[astro]->setTexture(theTextureMgr->getTexture(textureName[0]));
		theAsteroids[astro]->setSpriteDimensions(theTextureMgr->getTexture(textureName[0])->getTWidth(), theTextureMgr->getTexture(textureName[0])->getTHeight());
		theAsteroids[astro]->setAsteroidVelocity({ 3.0f, 3.0f });
		theAsteroids[astro]->setActive(true);
	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	// Render each asteroid in the vector array
	for (int draw = 0; draw < theAsteroids.size(); draw++)
	{
		theAsteroids[draw]->render(theRenderer, &theAsteroids[draw]->getSpriteDimensions(), &theAsteroids[draw]->getSpritePos(), theAsteroids[draw]->getSpriteRotAngle(), &theAsteroids[draw]->getSpriteCentre(), theAsteroids[draw]->getSpriteScale());
	}
	// Render each bullet in the vector array
	for (int draw = 0; draw < theBullets.size(); draw++)
	{
		theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
	}
	// render the rocket
	player[0]->render(theRenderer, &player[0]->getSpriteDimensions(), &player[0]->getSpritePos(), player[0]->getSpriteRotAngle(), &player[0]->getSpriteCentre(), player[0]->getSpriteScale());
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// Update the visibility and position of each asteriod
	vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
	while (asteroidIterator != theAsteroids.end())
	{
		if ((*asteroidIterator)->isActive() == false)
		{
			asteroidIterator = theAsteroids.erase(asteroidIterator);
		}
		else
		{
			(*asteroidIterator)->update(deltaTime);
			++asteroidIterator;
		}
	}
	// Update the visibility and position of each bullet
	vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
	while (bulletIterartor != theBullets.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = theBullets.erase(bulletIterartor);
		}
		else
		{
			(*bulletIterartor)->update(deltaTime);
			++bulletIterartor;

		}
	}
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
	{
		//(*bulletIterartor)->update(deltaTime);
		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
		{
			if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and asteroid to false
				(*asteroidIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
				score += 10;
			}
		}

	}

	for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
	{
		if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &player[0]->getBoundingRect()))
		{
			(*asteroidIterator)->setActive(false);
			(&theRocket)->setActive(false);
			std::string player_score = std::to_string(score);
			MessageBoxA(NULL, player_score.c_str() , "game over!", MB_OK);
			exit(0);


		}
	}


	// Update the Rockets position
	player[0]->update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					if (player[0]->getSpritePos().x < (renderWidth - player[0]->getSpritePos().w))
					{
						player[0]->setSpriteTranslation({ -10, -10 });
					}
				}
				break;

				case SDLK_UP:
				{
					if (player[0]->getSpritePos().x > 0)
					{
						player[0]->setSpriteTranslation({ 10, 10 });
					}
				}
				break;

				break;
				case SDLK_SPACE:
				{
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ player[0]->getBoundingRect().x + player[0]->getSpriteCentre().x, player[0]->getBoundingRect().y + player[0]->getSpriteCentre().y });
					theBullets[numBullets]->setSpriteTranslation({ 2.0f, 2.0f });
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					theBullets[numBullets]->setBulletVelocity({ 2.0f, 2.0f });
					theBullets[numBullets]->setSpriteRotAngle(player[0]->getSpriteRotAngle());
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << player[0]->getBoundingRect().x << " y: " << player[0]->getBoundingRect().y << endl;
				}
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}


double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}


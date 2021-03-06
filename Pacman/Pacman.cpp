#include "Pacman.h"

#include <sstream>

#include <time.h>


Pacman::Pacman(int argc, char* argv[], int munchieCount) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250), _cMunchieFrameTime(500), _cCherryFrameTime(500)
{
	munchiesCount = munchieCount;
	_munchies = new Enemy*[munchieCount];
	// Local variable
	int i;
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.png", false);
	for (i = 0; i < munchieCount; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frame = rand() % 500 + 50;
		_munchies[i]->blueTexture = munchieTex;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->collected = false;
	}
	int y;
	Texture2D* cherryTex = new Texture2D();
	cherryTex->Load("Textures/Cherry.png", false);
	for (y = 0; y < CHERRYCOUNT; y++)
	{
		_cherries[y] = new Cherry();
		_cherries[y]->frameCount1 = rand() % 1;
		_cherries[y]->currentFrameTime = 0;
		_cherries[y]->frame = rand() % 500 + 50;
		_cherries[y]->redTexture = cherryTex;
		_cherries[y]->rKeyDown = false;
		_cherries[y]->currentFrameTime = 0;
	}
	// Initialise member variables
	_pacman = new Player();
	_menu = new Menu();

	
	//_munchies[i]->frameCount = 0;
	//_cherries[y]->frameCount1 = 0;
	_menu->paused = false;
	_menu->won = false;
	_menu->lost = false;
	_menu->pKeyDown = false;
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultiplier = 1.0f;
	//_pacmanDirection = 0;
	//_pacmanCurrentFrameTime = 0;
	//_pacmanFrame = 0;
	
	_pop = new SoundEffect();
	_music = new SoundEffect();

	_musicStarted = false;

	//pacman
	_pacman->dead = false;

	//Initialise ghost character
	int x;
	Texture2D* ghostTex = new Texture2D();
	ghostTex->Load("Textures/GhostBlue.png", false);
	for (x = 0; x < GHOSTCOUNT; x++)
	{
		_ghosts[x] = new MovingEnemy();
		_ghosts[x]->direction = 0;
		_ghosts[x]->speed = 0.2f;
	}

	// Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25,
		"Pacman", 60);
	Input::Initialise();
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();

}

Pacman::~Pacman()
{
	//Clean up the Texture
	int nCount = 0;
	for (nCount = 0; nCount < munchiesCount; nCount++)
	{
		//delete _munchies[nCount]->position;
		delete _munchies[nCount]->rect;
		delete _munchies[nCount];
	}
	
	delete[] _munchies;

	//Clean up the Texture
	int nCountC = 0;
	for (nCountC = 0; nCountC < CHERRYCOUNT; nCountC++)
	{
		//delete _cherries[nCount]->position;
		delete _cherries[nCountC]->rect;
		delete _cherries[nCountC];
	}

	delete[] _cherries;
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;
	delete _pacman;
	delete _pop;
	//delete _munchies[i]->blueTexture;
	//delete _munchies[i]->invertedTexture;
	//delete _munchies[i]->rect;


}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	//_pacmanTexture = new Texture2D();
	//_pacmanTexture->Load("Textures/Pacman.tga", false);
	//_pacmanPosition = new Vector2(350.0f, 350.0f);
	//_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	int i;
	for (i = 0; i < munchiesCount; i++)
	{
		_munchies[i]->blueTexture = new Texture2D();
		_munchies[i]->blueTexture->Load("Textures/Munchie.tga", true);
		_munchies[i]->invertedTexture = new Texture2D();
		_munchies[i]->invertedTexture->Load("Textures/MunchieInverted.tga", true);
		_munchies[i]->rect = new Rect(100.0f, 450.0f, 12, 12);
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}

	// Load Cherry
	int y;
	for (y = 0; y < CHERRYCOUNT; y++)
	{
		_cherries[y]->redTexture = new Texture2D();
		_cherries[y]->redTexture->Load("Textures/CherrySingle.png", true);
		_cherries[y]->invertedTexture = new Texture2D();
		_cherries[y]->invertedTexture->Load("Textures/CherryInverted.png", true);
		_cherries[y]->rect = new Rect(50.0f, 450.0f, 32, 32);
		_cherries[y]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set score position
	_scorePosition = new Vector2(930.0f, 25.0f);

	// Set lives position
	_livesPosition = new Vector2(10.0f, (Graphics::GetViewportHeight()-25.0f));

	playerLives = 1;
	
	// Set Menu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f,
		Graphics::GetViewportHeight() / 2.0f);

	//Initialise ghost character
	int x;
	for (x = 0; x < GHOSTCOUNT; x++)
	{
		_ghosts[x]->texture = new Texture2D();
		_ghosts[x]->texture->Load("Textures/GhostBlue.png", false);
		_ghosts[x]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[x]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}

	_pop->Load("Sounds/pop.wav");
	_music->Load("Sounds/music.wav");
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	if (keyboardState->IsKeyDown(Input::Keys::SPACE))
	{
		_menu->start = true;
	}
	
	if (_menu->start && !_menu->won && !_menu->lost)
	{
		CheckPaused(keyboardState, Input::Keys::P);
		
		if (!_menu->paused)
		{
			Input(elapsedTime, keyboardState);
			UpdatePacman(elapsedTime);
			UpdateGhost(_ghosts[0], elapsedTime);
			CheckGhostCollisions(elapsedTime);
			Music();
			for (int i = 0; i < munchiesCount; i++)
			{
				UpdateMunchies(elapsedTime);
			}
			for (int i = 0; i < CHERRYCOUNT; i++)
			{
				UpdateCherries(elapsedTime);
			}
			CheckViewportCollision();
			CheckMunchieCollisions();
			CheckCherryCollisions();
		}

	}
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
	
	// Random Cherry
	int cherryMove = 0;
	if (keyboardState->IsKeyDown(Input::Keys::R) && cherryMove < 1)
	{
		int i = 1;
		float cherryPosX = rand() % (Graphics::GetViewportWidth() - 32) + 32;
		float cherryPosY = rand() % (Graphics::GetViewportHeight() - 32) + 32;
		_cherries[i]->rect = new Rect(cherryPosX, cherryPosY, 32, 32);
		cherryMove == 2;
	}
	if (keyboardState->IsKeyUp(Input::Keys::R))
	{
		cherryMove == 0;
	}
	

	// Speed Multiplier
	if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
	{
		// Apply Multiplier
		_pacman->speedMultiplier = 2.0f;
	}
	else
	{
		// Reset Multiplier
		_pacman->speedMultiplier = 1.0f;
	}
	
	// Checks if Right Arrow key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::RIGHT))
	{
		_pacman->position->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 0;
	}

	// Checks if Left Arrow key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::LEFT))
	{
		_pacman->position->X += -pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 2;
	}

	// Checks if Up Arrow key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::UP))
	{
		_pacman->position->Y += -pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 3;
	}

	// Checks if Down Arrow key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::DOWN))
	{
		_pacman->position->Y += pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 1;
	}
	// Checks if D key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 0;
	}

	// Checks if Left Arrow key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X += -pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 2;
	}

	// Checks if W key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y += -pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 3;
	}

	// Checks if S key is pressed
	else if (keyboardState->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed; //Moves Pacman across Y axis
		_pacman->direction = 1;
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	
	if (keyboardState->IsKeyDown(Input::Keys::P) && !_menu->pKeyDown)
	{
		_menu->pKeyDown = true;
		_menu->paused = !_menu->paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_menu->pKeyDown = false;

	if (munchieScore >= munchiesCount)
	{
		_menu->won = true;
	}

	if (_pacman->dead)
	{
		_menu->lost = true;
	}
}

void Pacman::UpdatePacman(int elapsedTime)
{
	_pacman->currentFrameTime += elapsedTime;

	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}
}

void Pacman::UpdateMunchies(int elapsedTime)
{
	int i;
	for (i = 0; i < munchiesCount; i++)
	{
		_munchies[i]->currentFrameTime += elapsedTime;

		if (_munchies[i]->currentFrameTime > _cMunchieFrameTime)
		{
			_munchies[i]->frameCount++;

			if (_munchies[i]->frameCount >= 2)
				_munchies[i]->frameCount = 0;

			_munchies[i]->currentFrameTime = 0;
		}
	}
}

void Pacman::UpdateCherries(int elapsedTime)
{
	int i;
	for (i = 0; i < CHERRYCOUNT; i++)
	{
		_cherries[i]->currentFrameTime += elapsedTime;

		if (_cherries[i]->currentFrameTime > _cCherryFrameTime)
		{
			_cherries[i]->frameCount1++;

			if (_cherries[i]->frameCount1 >= 2)
				_cherries[i]->frameCount1 = 0;

			_cherries[i]->currentFrameTime = 0;
		}
	}
}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime)
{
	int i;
	for (i = 0; i < GHOSTCOUNT; i++)
	{
		if (_ghosts[i]->direction == 0) //Moves Right
		{
			_ghosts[i]->position->X += _ghosts[i]->speed * elapsedTime;
		}
		else if (_ghosts[i]->direction == 1) // Moves Left
		{
			_ghosts[i]->position->X -= _ghosts[i]->speed * elapsedTime;
		}

		if (_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width >= Graphics::GetViewportWidth()) // Hits Right Edge
		{
			_ghosts[i]->direction = 1; // Change direction
		}
		else if (_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width <= 0) // Hits Right Edge
		{
			_ghosts[i]->direction = 0; // Change direction
		}
	}
}

void Pacman::CheckGhostCollisions(int elapsedTime)
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		// Populate variables with ghost data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 <right2))
		{

			if (playerLives <= 1)
			{
				playerLives--;
				_pacman->dead = true;
			}
			else if (playerLives > 1)
			{
				playerLives--;
			}

			i = GHOSTCOUNT;
			
		}
	}
}

void Pacman::CheckMunchieCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < munchiesCount; i++)
	{
		// Populate variables with ghost data
		bottom2 = _munchies[i]->position->Y + _munchies[i]->rect->Height;
		left2 = _munchies[i]->position->X;
		right2 = _munchies[i]->position->X + _munchies[i]->rect->Width;
		top2 = _munchies[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2) && _munchies[i]->collected == false)
		{
			//_pacman->dead = true;
			if (!Audio::IsInitialised())
			{
				std::cout << "Audio is not initialised" << std::endl;
			}
			if (!_pop->IsLoaded())
			{
				std::cout << "_pop member sound efect has not loaded" << std::endl;
			}
			Audio::Play(_pop);
			//_munchies[i]->blueTexture->Load("Textures/blank.png", true);
			//_munchies[i]->invertedTexture->Load("Textures/blank.png", true);
			_munchies[i]->collected = true;
			i = munchiesCount;
			munchieScore++;
		}
	}
}

void Pacman::CheckCherryCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < CHERRYCOUNT; i++)
	{
		// Populate variables with ghost data
		bottom2 = _cherries[i]->position->Y + _cherries[i]->rect->Height;
		left2 = _cherries[i]->position->X;
		right2 = _cherries[i]->position->X + _cherries[i]->rect->Width;
		top2 = _cherries[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2) && _cherries[i]->collected == false)
		{
			if (!Audio::IsInitialised())
			{
				std::cout << "Audio is not initialised" << std::endl;
			}
			if (!_pop->IsLoaded())
			{
				std::cout << "_pop member sound efect has not loaded" << std::endl;
			}
			Audio::Play(_pop);
			//_munchies[i]->blueTexture->Load("Textures/blank.png", true);
			//_munchies[i]->invertedTexture->Load("Textures/blank.png", true);
			_cherries[i]->collected = true;
			i = CHERRYCOUNT;
			playerLives++;
		}
	}
}


void Pacman::Music()
{
	if (_musicStarted == false)
	{
		if (!Audio::IsInitialised())
		{
			std::cout << "Audio is not initialised" << std::endl;
		}
		if (!_music->IsLoaded())
		{
			std::cout << "_music member sound effect has not loaded" << std::endl;
		}
		Audio::Play(_music);
		_musicStarted = true;
	}
	if (_pacman->dead)
	{
		Audio::Stop(_music);
	}

	if (_menu->paused)
	{
		Audio::Stop(_music);
	}
}

void Pacman::CheckViewportCollision()
{
	// Checks if Pacman is trying to dissapear
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth()) //1024 is game width
	{
		// Pacman hit right wall - reset his position
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	}

	// Checks if Pacman is trying to dissapear
	if (_pacman->position->X + _pacman->sourceRect->Width < _SpriteWidth) //32 is value of left wall
	{
		// Pacman hit left wall - reset his position
		_pacman->position->X = _SpriteWidth - _pacman->sourceRect->Width;
	}

	// Checks if Pacman is trying to dissapear
	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight()) //768 is bottom of map
	{
		// Pacman hit right wall - reset his position
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
	}

	// Checks if Pacman is trying to dissapear
	if (_pacman->position->Y + _pacman->sourceRect->Height < _SpriteWidth) //32 is top of map
	{
		// Pacman hit right wall - reset his position
		_pacman->position->Y = _SpriteWidth - _pacman->sourceRect->Height;
	}

	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
}


void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	// Allows us to easily create a string
	std::stringstream score;
	score << "Score: " << munchieScore;

	std::stringstream lives;
	lives << "Lives: " << playerLives;

	SpriteBatch::BeginDraw(); // Starts Drawing
	if (!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}

	int x;

	for (x = 0; x < GHOSTCOUNT; x++)
	{
		SpriteBatch::Draw(_ghosts[x]->texture, _ghosts[x]->position, _ghosts[x]->sourceRect);
	}

	//Texture2D* texture = new Texture2D();
	//Vector2* position = new Vector2();
	//position = new Vector2(450.0f, 450.0f);
	//texture->Load("Textures/Munchie.png", true);
	//Rect* sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	//SpriteBatch::Draw(texture, position, sourceRect);

	int i;
	
	for (i = 0; i < munchiesCount; i++)
	{
		if (_munchies[i]->collected == false)
		{
			if (_munchies[i]->frameCount == 0)
			{
				// Draws Red Munchie
				SpriteBatch::Draw(_munchies[i]->invertedTexture, _munchies[i]->position, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				//_frameCount++;
			}
			else
			{
				// Draw Blue Munchie
				SpriteBatch::Draw(_munchies[i]->blueTexture, _munchies[i]->position, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				//_frameCount++;

				//if (_frameCount >= 60)
				//	_frameCount = 0;
			}
		}
	}
	
	int y;
	for (y = 0; y < CHERRYCOUNT; y++)
	{
		if (_cherries[y]->collected == false)
		{
			if (_cherries[y]->frameCount1 == 0)
			{
				// Draws Blue Cherry
				SpriteBatch::Draw(_cherries[y]->invertedTexture, _cherries[y]->position, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				//_frameCount++;
			}
			else
			{
				// Draw Red Cherry
				SpriteBatch::Draw(_cherries[y]->redTexture, _cherries[y]->position, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

				//_frameCount++;

				//if (_frameCount >= 60)
				//	_frameCount = 0;
			}
		}
	}
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawString(score.str().c_str(), _scorePosition, Color::Green);
	//SpriteBatch::DrawString(lives.str().c_str(), _livesPosition, Color::Red);

	if (_menu->paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition,
			Color::Red);
	}

	if (!_menu->start)
	{
		std::stringstream menuStream;
		menuStream << "Press SPACE to start!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition,
			Color::Green);
	}

	if (_menu->won)
	{
		std::stringstream menuStream;
		menuStream << "You Won!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition,
			Color::Green);
	}

	if (_menu->lost)
	{
		std::stringstream menuStream;
		menuStream << "You Lost!";

		SpriteBatch::Draw(_menu->background, _menu->rectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition,
			Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}



//Change code in CheckViewportCollision() to code below to allow pacman to cross to otherside of screen instead of colliding

/* Checks if Pacman is trying to dissapear
This code is for wrap around the map */
//if (_pacmanPosition->X + _pacmanSourceRect->Width > 1044) //1024 is game width
//{
	// Pacman hit right wall - reset his position
	//_pacmanPosition->X = 0 - _pacmanSourceRect->Width;
//}

/* Checks if Pacman is trying to dissapear
This code is for wrap around the map */
//if (_pacmanPosition->X + _pacmanSourceRect->Width < 0) //1024 is game width
//{
	// Pacman hit right wall - reset his position
	//_pacmanPosition->X = 1044 - _pacmanSourceRect->Width;
//}

/* Checks if Pacman is trying to dissapear
This code is for wrap around the map */
//if (_pacmanPosition->Y + _pacmanSourceRect->Height > 788) //768 is bottom of map
//{
	// Pacman hit right wall - reset his position
	//_pacmanPosition->Y = 12 - _pacmanSourceRect->Height;
//}

/* Checks if Pacman is trying to dissapear
This code is for wrap around the map */
//if (_pacmanPosition->Y + _pacmanSourceRect->Height < 12) //768 is bottom of map
//{
	// Pacman hit right wall - reset his position
	//_pacmanPosition->Y = 788 - _pacmanSourceRect->Height;
//}
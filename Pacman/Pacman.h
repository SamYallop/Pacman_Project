#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include <iostream>

#define MUNCHIECOUNT 50
#define CHERRYCOUNT 50
#define GHOSTCOUNT 10

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Structure Definition

struct Player
{
	bool dead;
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

struct Enemy
{
	int  frameCount;
	Rect* rect;
	Texture2D* blueTexture;
	Texture2D* invertedTexture;
	int frame;
	int currentFrameTime;
	Vector2* position;
	bool collected;
};

struct Cherry
{
	int  frameCount1;
	Rect* rect;
	Texture2D* redTexture;
	Texture2D* invertedTexture;
	int frame;
	int currentFrameTime;
	bool rKeyDown;
};

struct Menu
{
	Texture2D* background;
	Rect* rectangle;
	Vector2* stringPosition;
	bool paused;
	bool pKeyDown;
	bool start;
};

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
	int  frameCount;
};



// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state);
	//Check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckStart(Input::KeyboardState* state, Input::Keys startKey);
	void CheckViewportCollision();
	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchies(int elapsedTime);
	void CheckMunchieCollisions();
	void UpdateCherries(int elapsedTime);
	void Music();

	void CheckGhostCollisions();
	void UpdateGhost(MovingEnemy*, int elapsedTime);
	// Constant data for Game Variables
	const float _cPacmanSpeed;

	// Constant for pacman width
	const int _SpriteWidth = 32;

	Player* _pacman;
	Enemy** _munchies;
	Menu* _menu;
	Cherry* _cherries[CHERRYCOUNT];
	MovingEnemy* _ghosts[GHOSTCOUNT];

	// Data to represent Pacman
	const int _cPacmanFrameTime;

	// Data to represent Munchie
	const int _cMunchieFrameTime;

	// Data to represent Cherry
	const int _cCherryFrameTime;

	// Position for String
	Vector2* _stringPosition;

	bool _musicStarted;

	int munchieCount;

	//bool _munchieCollected[munchieCount];

	SoundEffect* _pop;

	SoundEffect* _music;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[], int munchieCount);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

};
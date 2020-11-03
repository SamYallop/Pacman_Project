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

#define MUNCHIECOUNT 50

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Structure Definition

struct Player
{
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

	// Constant data for Game Variables
	const float _cPacmanSpeed;

	// Constant for pacman width
	const int _SpriteWidth = 32;

	Player* _pacman;
	Enemy* _munchies[MUNCHIECOUNT];
	Menu* _menu;

	// Data to represent Pacman
	const int _cPacmanFrameTime;

	// Data to represent Munchie
	const int _cMunchieFrameTime;

	// Position for String
	Vector2* _stringPosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

};
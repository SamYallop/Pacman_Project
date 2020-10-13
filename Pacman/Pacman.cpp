#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f)
{
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25,
		"Pacman", 60);
	Input::Initialise();
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(),
		Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f,
		Graphics::GetViewportHeight() / 2.0f);


}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (!_paused)
	{


		// Checks if Right Arrow key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::RIGHT))
			_pacmanPosition->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis

		// Checks if Left Arrow key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::LEFT))
			_pacmanPosition->X += -_cPacmanSpeed * elapsedTime; //Moves Pacman across X axis

		// Checks if Up Arrow key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::UP))
			_pacmanPosition->Y += -_cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis

		// Checks if Down Arrow key is pressed
		if (keyboardState->IsKeyDown(Input::Keys::DOWN))
			_pacmanPosition->Y += _cPacmanSpeed * elapsedTime; //Moves Pacman across Y axis

		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->X + _pacmanSourceRect->Width > Graphics::GetViewportWidth()) //1024 is game width
		{
			// Pacman hit right wall - reset his position
			_pacmanPosition->X = Graphics::GetViewportWidth() - _pacmanSourceRect->Width;
		}

		/* Checks if Pacman is trying to dissapear
		This code is for wrap around the map */
		//if (_pacmanPosition->X + _pacmanSourceRect->Width > 1044) //1024 is game width
		//{
			// Pacman hit right wall - reset his position
			//_pacmanPosition->X = 0 - _pacmanSourceRect->Width;
		//}

		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->X + _pacmanSourceRect->Width < _SpriteWidth) //32 is value of left wall
		{
			// Pacman hit left wall - reset his position
			_pacmanPosition->X = _SpriteWidth - _pacmanSourceRect->Width;
		}

		/* Checks if Pacman is trying to dissapear
		This code is for wrap around the map */
		//if (_pacmanPosition->X + _pacmanSourceRect->Width < 0) //1024 is game width
		//{
			// Pacman hit right wall - reset his position
			//_pacmanPosition->X = 1044 - _pacmanSourceRect->Width;
		//}

		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->Y + _pacmanSourceRect->Height > Graphics::GetViewportHeight()) //768 is bottom of map
		{
			// Pacman hit right wall - reset his position
			_pacmanPosition->Y = Graphics::GetViewportHeight() - _pacmanSourceRect->Height;
		}

		/* Checks if Pacman is trying to dissapear
		This code is for wrap around the map */
		//if (_pacmanPosition->Y + _pacmanSourceRect->Height > 788) //768 is bottom of map
		//{
			// Pacman hit right wall - reset his position
			//_pacmanPosition->Y = 12 - _pacmanSourceRect->Height;
		//}

		// Checks if Pacman is trying to dissapear
		if (_pacmanPosition->Y + _pacmanSourceRect->Height < _SpriteWidth) //32 is top of map
		{
			// Pacman hit right wall - reset his position
			_pacmanPosition->Y = _SpriteWidth - _pacmanSourceRect->Height;
		}

		/* Checks if Pacman is trying to dissapear
		This code is for wrap around the map */
		//if (_pacmanPosition->Y + _pacmanSourceRect->Height < 12) //768 is bottom of map
		//{
			// Pacman hit right wall - reset his position
			//_pacmanPosition->Y = 788 - _pacmanSourceRect->Height;
		//}
		_frameCount++;
	}

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;

}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		//_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		
		//_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition,
			Color::Red);
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}
#pragma once
#include "Game.h"
#include "TextureManager.h"

class GameState
{
public:
	GameState(Game& game);

	/*
	handInput method:
	This method is responsible for handling all the input for the current gamestate
	This should not know about time, merely the events that have occured.
	It should apply all the events here as well, not store for later.
	*/
	virtual void handleInput() = 0;

	/*
	Update method:
	const float dt: This is the delta time passed since the last update in seconds, represented as a float
	This is where we now update our physical model, this includes any new objects created curing handleInput
	*/
	virtual void update(const float dtSeconds) = 0;

	/*
	draw method:
	This method is responsible for drawing the current gamestate

	*/
	virtual void draw() = 0;

	bool isFinished();

	TextureManager& getTextureManager();
	
protected:
	Game& game;					// the game, required to be able to update the state, and possible any settings
	sf::RenderWindow& window;	// the main render window, stored as a reference from the Game
	sf::Event event;
	TextureManager textureManager;

	void setFinished();
private:
	bool finished;
};


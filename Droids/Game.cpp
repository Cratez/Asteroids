#include "Game.h"
#include "Constants.h"
#include "GameState.h"

Game::Game() :
	window(sf::VideoMode(VIDEOMODE), "AsterDroids - a silly space game"),
	dt(sf::seconds(0))
{
	window.setFramerateLimit(60);
}

void Game::pushState(GameState* state)
{
	states.push(state);
}

void Game::changeState(GameState* state)
{
	popState();	
	states.push(state);
}

void Game::popState()
{
	if (!states.empty()) {
		delete states.top();
		states.pop();
	}
}

GameState* Game::topState()
{
	if (states.empty())
		return nullptr;

	return states.top();
}

void Game::start()
{
	//this is to do any local init stuff apart from constructor, a no-op for now
	gameLoop();
}

sf::RenderWindow & Game::getWindow()
{
	return window;
}

void Game::gameLoop()
{
	sf::Clock clock;

	GameState* currentState;
	while (window.isOpen())
	{
		//get the current state we have
		currentState = topState();

		//ensure we are not stateless somehow
		if (currentState == nullptr)
			continue; // should this ever really happen?

		//get the time elapsed
		dt = clock.restart();

		//handle the input
		currentState->handleInput();

		//update the model with the seconds that have passed
		currentState->update(dt.asSeconds());

		//draw the state
		currentState->draw();

		if (currentState->isFinished())
			popState();
	}
}

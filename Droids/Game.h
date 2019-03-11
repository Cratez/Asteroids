#pragma once
#include <stack>
#include <SFML/Graphics.hpp>
#include <memory>


using namespace std;

class GameState;

class Game
{
public:
	Game();

	void pushState(GameState* state);
	void changeState(GameState* state);
	void popState();
	GameState* topState();
	void start();
	sf::RenderWindow& getWindow();

private:
	stack<GameState*> states;
	sf::RenderWindow window;
	sf::Time dt;
	void gameLoop();
};


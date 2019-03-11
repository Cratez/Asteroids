#pragma once
#include "GameState.h"
#include "Game.h"

class PauseState : public GameState
{
public:
	PauseState(Game& game);

	virtual void handleInput();
	virtual void update(const float dtSeconds);
	virtual void draw();


private:
	sf::View fixedView;
	sf::RectangleShape filterScreen;
	sf::Texture originalScreenTexture;
	sf::Sprite originalScreenSprite;
};


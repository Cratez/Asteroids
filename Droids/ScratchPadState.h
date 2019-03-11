#pragma once
#include "GameState.h"
#include "StarField.h"
#include "TextureManager.h"
#include "ShipObject.h"
#include <vector>

class ScratchPadState : public GameState
{
public:
	ScratchPadState(Game& game);
	void draw() override;
	void update(float dtSeconds) override;
	void handleInput() override;

private:
	const float CAMERA_SPEED = 100;
	sf::View standardView;
	sf::View fixedView;
	StarField starField;
	b2World world;
	ShipObject ship;

	sf::Text shipCoordText;
	sf::CircleShape test;
};


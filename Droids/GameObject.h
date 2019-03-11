#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "GameState.h"
#include "TextureManager.h"
#include "Common.h"

class GameObject : public sf::Drawable
{
public:
	GameObject(GameState& gameState, b2World& world, sf::Vector2f initCoords, float initRotation);
	~GameObject();

	//overload these if you need to control update logic
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;	
	virtual void update(float dtSeconds);

	void setHidden(bool trueFalse);
	bool isHidden();

	
protected:
	void updateSFMLObject();

protected:
	//member data
	const float INIT_ROTATION;
	const sf::Vector2f INIT_COORDS;

	bool mHidden;
	sf::Sprite mSprite;

	b2Body* mBody;
	b2World& mWorld;
	GameState& mGameState;
	TextureManager& mTextureManager;
};


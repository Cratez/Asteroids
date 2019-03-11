#pragma once
#include "GameObject.h"
#include "Constants.h"
#include <vector>

class ShipObject : public GameObject
{
public:
	ShipObject(GameState& gameState, b2World& theWorld, sf::Vector2f initCoords = sf::Vector2f(500,500), float initRotation = 0);

	void setForceVector(sf::Vector2f force);
	void rotate(float degrees);
	virtual void update(float dtSeconds) override;
	void setForceScaler(unsigned force);
	sf::Vector2f getDeltaMovement();
	sf::Vector2f getPosition();

protected:
	void buildB2Body();

	unsigned mForceScaler;
	bool mIsForceVectorStale;
	b2Vec2 mForceVector;
	b2Vec2 mLastPos;
	b2Vec2 mDeltaPos;
	std::vector<sf::Vector2f> mPoints;
	
};


#include "gameObject.h"


GameObject::GameObject(
	GameState & gameState,	// the gamestate we are in
	b2World & world,		// the physics game world
	sf::Vector2f initCoords,// init coords, supplied by child classes
	float initRotation		// rotation amount, supplied by child classes
) :
	mGameState(gameState), mWorld(world), 
	INIT_COORDS(initCoords), INIT_ROTATION(initRotation),
	mTextureManager(gameState.getTextureManager()),
	mBody(nullptr)
{
	mSprite.setPosition(INIT_COORDS);
	mSprite.setRotation(INIT_ROTATION);
}

GameObject::~GameObject()
{
	if (mBody != nullptr) {
		mWorld.DestroyBody(mBody);
		mBody = nullptr;
	}
}

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (!mHidden)
		target.draw(mSprite);
}

void GameObject::update(float dtSeconds)
{
	updateSFMLObject();
}

void GameObject::updateSFMLObject()
{
	if (mBody != nullptr) {
		mSprite.setPosition(coordsWorldToPixel(mBody->GetPosition()));
		mSprite.setRotation(radiansToDegrees(mBody->GetAngle()));
	}	
}

void GameObject::setHidden(bool trueFalse)
{
	mHidden = trueFalse;
}

bool GameObject::isHidden()
{
	return mHidden;
}


#include "ShipObject.h"


ShipObject::ShipObject(GameState& gameState, b2World & theWorld, sf::Vector2f initCoords, float initRotation) :
GameObject(gameState, theWorld, initCoords, initRotation), 
mIsForceVectorStale(false), mForceScaler(0), mForceVector(0,0), mLastPos(coordsPixelToWorld(initCoords))
{
	mForceVector = b2Vec2(0, 0);
	//if (!mTextureManager.hasTexture("ship")) {
	//	mTextureManager.loadTexture("ship", "ship.png");
	//}
	//mSprite.setTexture(mTextureManager.getTextureRef("ship"));

	mPoints.push_back(sf::Vector2f(1 * METERS_TO_PIXELS,0));
	mPoints.push_back(sf::Vector2f(2 * METERS_TO_PIXELS, 3 * METERS_TO_PIXELS));
	mPoints.push_back(sf::Vector2f(0, 3 * METERS_TO_PIXELS));

	sf::ConvexShape polygon;
	polygon.setPointCount(3);
	polygon.setPoint(0, mPoints[0]);
	polygon.setPoint(1, mPoints[1]);
	polygon.setPoint(2, mPoints[2]);
	polygon.setFillColor(sf::Color::White);

	sf::RenderTexture renderer;
	renderer.create(10 * METERS_TO_PIXELS, 10 * METERS_TO_PIXELS);
	renderer.clear();
	renderer.draw(polygon);
	renderer.display();

	sf::Texture txt = renderer.getTexture();
	sf::Image img = txt.copyToImage();
	img.createMaskFromColor(sf::Color::Black);
	txt.update(img);
	
	mTextureManager.addTexture("ship", txt);
	mSprite.setTexture(mTextureManager.getTextureRef("ship"));

	buildB2Body();
}

void ShipObject::setForceVector(sf::Vector2f force)
{
	mForceVector = toB2Vect2(force);
}

void ShipObject::rotate(float degrees)
{
	mBody->SetTransform(mBody->GetPosition(), degreesToRadians(degrees));
}


void ShipObject::update(float dtSeconds)
{
	GameObject::update(dtSeconds);
	mBody->ApplyForce(b2Vec2(0, -150), mBody->GetWorldCenter(),false);
	mDeltaPos = mBody->GetPosition() - mLastPos;
	mLastPos = mBody->GetPosition();
	//b2Vec2 force = mForceVector;
	//force *= dtSeconds;
	//mBody->ApplyForce(force,b2Vec2(0,0),false);
}

void ShipObject::setForceScaler(unsigned force)
{
	b2Vec2 t(1, 1);
	t *= 1;
	if (force != mForceScaler) {
		mForceScaler = force;
		mIsForceVectorStale = true;
	}	
}

sf::Vector2f ShipObject::getDeltaMovement()
{
	return coordsWorldToPixel(mDeltaPos);
}

sf::Vector2f ShipObject::getPosition()
{
	return mSprite.getPosition();
}

void ShipObject::buildB2Body()
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(coordsPixelToWorld(INIT_COORDS));
	BodyDef.type = b2_dynamicBody;
	mBody = mWorld.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((10.f / 2), (15.f / 2));
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	mBody->CreateFixture(&FixtureDef);
}

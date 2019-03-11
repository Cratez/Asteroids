#include "StarField.h"
#include <sstream>

StarField::Star::Star(sf::Vector2f location, star_size size) {
	this->location = location;
	this->size = size;
}
void StarField::Star::draw(sf::RenderTarget& window, sf::CircleShape& starShape) {
	starShape.setRadius(static_cast<float>(size));
	starShape.setPosition(location);
	window.draw(starShape);
}

void StarField::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::View preDrawView;
	if (mHaveFixedView) {
		preDrawView = target.getView();
		target.setView(mFixedView);
	}

	for (unsigned i = 0; i <= mMovingFieldsCount; i++) {
		if (mDrawLayer[i] && mLayerReady[i]) {
			target.draw(mStarFieldSprites[i]);
		}
	}

	if (mHaveFixedView) {
		target.setView(preDrawView);
	}
}

StarField::StarField(sf::View fixedView, unsigned movingStarFieldCount) :
	StarField(fixedView.getSize(), movingStarFieldCount)
{
	setFixedView(fixedView);
}

StarField::StarField(sf::Vector2f screenSize, unsigned movingStarFieldCount) :
	mMovingFieldsCount(movingStarFieldCount),
	//mWindow(window),
	mScreenSize(screenSize),
	mSeed(static_cast<int>(chrono::system_clock::now().time_since_epoch().count())),
	mHaveFixedView(false),
	mMovementScale(DEFAULT_MOVEMENT_SCALE)
{
	mLayerReady[MAX_MOVING_STAR_FIELDS] = false;
	mDrawLayer[MAX_MOVING_STAR_FIELDS] = true;
	for (unsigned i = 0; i < MAX_MOVING_STAR_FIELDS; ++i) {
		mDrawMovingField[i] = false;
		mLayerReady[i] = false;
		mDrawLayer[i] = true;
	}

	if (mMovingFieldsCount > MAX_MOVING_STAR_FIELDS) {
		mMovingFieldsCount = MAX_MOVING_STAR_FIELDS;
	}

	initRandomGenerators();
}

void StarField::generate()
{
	for (unsigned i = 0; i <= mMovingFieldsCount; i++) {
		generate(i);
	}
}

void StarField::generate(unsigned layer)
{
	//ensure we dont render a field we cant use
	if (layer > mMovingFieldsCount)
		return;

	vector<Star>& theField = mStarFields[layer];
	theField.clear();
	sf::Vector2f deltaStar;
	float starbuffer;

	//calculate how many draw attempts to do
	unsigned maxAttempts = STAR_DRAW_ATTEMPTS_BASE - layer * STAR_DRAW_DECREASE_PER_LAYER;
	if (layer == BACKGROUND_STAR_INDEX) { // if drawing background
		maxAttempts = STAR_DRAW_ATTEMPTS_BACKGROUND;
	}
	else if (maxAttempts < STAR_DRAW_ATTEMPTS_MIN || maxAttempts > STAR_DRAW_ATTEMPTS_BASE) {
		maxAttempts = STAR_DRAW_ATTEMPTS_MIN;
	}


	star_size defaultSize = star_size::small; //the default
	if (layer == 1 || layer == 2) {
		defaultSize = star_size::small_med;
	}
	else if (layer >= 3) {
		defaultSize = star_size::medium;
	}

	for (unsigned attempts = 0; attempts < maxAttempts; attempts++)
	{
		star_size size = defaultSize;
		starbuffer = mStarSpacerDistribution(mGenerator);
		sf::Vector2f newStar(mDistributionX(mGenerator), mDistributionY(mGenerator));

		bool overlap = false;
		for (auto star : theField) {
			deltaStar = newStar - star.location;
			if (sqrt(deltaStar.x*deltaStar.x + deltaStar.y*deltaStar.y) < starbuffer)
			{
				overlap = true;
				break;
			}
		}

		if (overlap == false)
		{
			float rand = mStandardDistribution(mStandardGenerator);

			if (rand > EXTRA_LARGE) {
				size = star_size::extra_large;
			}
			else if (rand > LARGE && size < star_size::large) {
				size = star_size::large;
			}
			else if (rand > MED_LARGE  && size < star_size::med_large) {
				size = star_size::med_large;
			}
			else if (rand > MEDIUM  && size < star_size::medium) {
				size = star_size::medium;
			}
			else if (rand > SMALL_MED && size < star_size::small_med) {
				size = star_size::small_med;
			}

			theField.push_back(Star(newStar, size));
		}
	}

	drawStarField(layer);
}

void StarField::move(sf::Vector2f movement)
{
	float movementScale = mMovementScale * 2;
	for (unsigned i = mMovingFieldsCount; i != 0; --i)
	{
		movementScale = movementScale * 0.5f;
		mStarFieldSprites[i].move(movement.x * movementScale, movement.y * movementScale);
		//check the X
		if (mStarFieldSprites[i].getPosition().x < -mScreenSize.x || mScreenSize.x < mStarFieldSprites[i].getPosition().x)
		{
			mStarFieldSprites[i].setPosition(0, mStarFieldSprites[i].getPosition().y);
		}

		//check the y
		if (mStarFieldSprites[i].getPosition().y < -mScreenSize.y || mScreenSize.y < mStarFieldSprites[i].getPosition().y)
		{
			mStarFieldSprites[i].setPosition(mStarFieldSprites[i].getPosition().x, 0);
		}
	}
}

void StarField::saveTextureToFile(unsigned layer, string starFieldFilePath)
{
	sf::Image img = mStarFieldTextures[layer].copyToImage();
	img.saveToFile(starFieldFilePath);
}

void StarField::loadTextureFromFile(unsigned layer, string starFieldFilePath)
{
	mStarFieldTextures[layer].loadFromFile(starFieldFilePath);
	mStarFieldTextures[layer].setRepeated(true);

	if (layer != 0) {
		//if not layer 0, make it 3x3 size
		mStarFieldSprites[layer] = sf::Sprite(mStarFieldTextures[layer], sf::IntRect(0, 0, static_cast<int>(mScreenSize.x) * 3, static_cast<int>(mScreenSize.y) * 3));
		mStarFieldSprites[layer].setOrigin(mScreenSize.x, mScreenSize.y);
	}
	else {
		mStarFieldSprites[layer].setTexture(mStarFieldTextures[layer]);
	}
	mLayerReady[layer] = true;
}

void StarField::setFixedView(sf::View view)
{
	mFixedView = view;
	mHaveFixedView = true;
}

void StarField::clearFixedView()
{
	mHaveFixedView = false;
}

float StarField::getMovementScale()
{
	return mMovementScale;
}

void StarField::setMovementScale(float scale)
{
	//we store the scale as negative so we can easier make it go the other way
	if (scale > 0)
		mMovementScale = scale * -1.f;
	else
		mMovementScale = scale;
}

unsigned StarField::getMovingLayerCount()
{
	return mMovingFieldsCount;
}

void StarField::setMovingStarFieldCount(unsigned layerCount)
{
	if (layerCount > MAX_MOVING_STAR_FIELDS) {
		mMovingFieldsCount = MAX_MOVING_STAR_FIELDS;
	}
	else {
		mMovingFieldsCount = layerCount;
	}
}

unsigned StarField::getTotalLayersCount()
{
	return mMovingFieldsCount + 1;
}

void StarField::setScreenSize(sf::Vector2f size)
{
	mScreenSize = size;
	initRandomGenerators();
}

bool StarField::isLayerReady(unsigned layer)
{
	return mLayerReady[layer];
}

bool StarField::isLayerEnabled(unsigned layer)
{
	return mDrawLayer[layer];
}

bool StarField::toggleLayerEnabled(unsigned layer)
{
	mDrawLayer[layer] = !mDrawLayer[layer];
	return mDrawLayer[layer];
}

void StarField::enableDisableLayer(unsigned layer, bool enableDisable)
{
	mDrawLayer[layer] = enableDisable;
}

void StarField::drawStarField(int layer)
{
	sf::RenderTexture texture;
	texture.create(static_cast<int>(mScreenSize.x), static_cast<int>(mScreenSize.y));

	vector<Star>& theField = mStarFields[layer];
	for (auto star : theField) {
		star.draw(texture, mStarShape);
	}
	texture.display();

	//create the texture
	mStarFieldTextures[layer] = texture.getTexture();

	//make background transparent
	sf::Image img = mStarFieldTextures[layer].copyToImage();
	img.createMaskFromColor(sf::Color::Black);

	//update it to the transparent background, and set to repeat
	mStarFieldTextures[layer].update(img);
	mStarFieldTextures[layer].setRepeated(true);

	if (layer != 0) {
		//if not layer 0, make it 3x3 size
		mStarFieldSprites[layer] = sf::Sprite(mStarFieldTextures[layer], sf::IntRect(0, 0, static_cast<int>(mScreenSize.x) * 3, static_cast<int>(mScreenSize.y) * 3));
		mStarFieldSprites[layer].setOrigin(mScreenSize.x, mScreenSize.y);
	}
	else {
		mStarFieldSprites[layer].setTexture(mStarFieldTextures[layer]);
	}

	mLayerReady[layer] = true;

	stringstream sstream;
	sstream << "starfield" << layer << ".png";
	img.saveToFile(sstream.str());
}

void StarField::initRandomGenerators()
{
	mGenerator = default_random_engine(mSeed);
	mStandardGenerator = default_random_engine(mSeed);
	mDistributionX = uniform_real_distribution<float>(0, mScreenSize.x);
	mDistributionY = uniform_real_distribution<float>(0, mScreenSize.y);
	mStarSpacerDistribution = uniform_real_distribution<float>(0, STAR_SPACER);
	mStandardDistribution = uniform_real_distribution<float>(0.0f, 1.0f);
}

#pragma once
/*
Class StarField:
Written by John Harrison
Description: This is a dynamic SFML starfield class intended to be very performance minded.
Starfields are converted to textures which are drawn as sprite to reduce the overhead of draw calls.
*/

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <Random>
#include <chrono>
#include "Common.h"

using namespace std;

#define MAX_MOVING_STAR_FIELDS 5


class StarField : public sf::Drawable
{
private:
	//consts
	const float EXTRA_LARGE = 0.995f;
	const float LARGE = 0.98f;
	const float MED_LARGE = 0.95f;
	const float MEDIUM = 0.90f;
	const float SMALL_MED = 0.50f;
	const float STAR_SPACER = 10.0f;
	const unsigned STAR_DRAW_ATTEMPTS_BACKGROUND = 500;
	const unsigned STAR_DRAW_ATTEMPTS_BASE = 200;
	const unsigned STAR_DRAW_ATTEMPTS_MIN = 10;
	const unsigned STAR_DRAW_DECREASE_PER_LAYER = 0;
	const unsigned BACKGROUND_STAR_INDEX = 0;
	const float DEFAULT_MOVEMENT_SCALE = -0.05f;

	//private class definition
	enum star_size { small = 1, small_med = 2, medium = 3, med_large = 4, large = 5, extra_large = 7 };
	class Star {
	public:
		//data
		sf::Vector2f location;
		star_size size;

		//methods
		Star(sf::Vector2f location, star_size size);
		void draw(sf::RenderTarget& window, sf::CircleShape& starShape);
	};

public:
	//implement the SFML drawable class to allow this class to be drawn normally
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	//constructor from a file
	StarField(sf::View fixedView, unsigned movingStarFieldCount = 0);
	StarField(sf::Vector2f screenSize, unsigned movingStarFieldCount = 0);

	//Generates the textures and sets the sprites for all layers
	void generate();

	//generates the texture and sets the sprite for the layer in quest, 0 is background, STARFIELD_MAX_COUNT is closest
	void generate(unsigned layer);

	/*
	This will move the star fields in the following fashion:
	The highest layer will move at movementScale (Default 1/30 of requested)
	Each layer under this that is not the base static layer, will move at 1/2 the scale of the above
	Base layer, or static layer does not move
	*/
	void move(sf::Vector2f movement);


	//Will save star field texture to the following file.
	void saveTextureToFile(unsigned layer, string starFieldFilePath);

	//will load a field from the following location and set it to the layer
	void loadTextureFromFile(unsigned layer, string starFieldFilePath);

	//==================getters and setters//==================

	//Set a fixed view to use when drawing the starfield
	void setFixedView(sf::View view);

	//Clears the fixed view if any
	void clearFixedView();

	//get the movementScale, this should be a value  (0.f,1.f]
	float getMovementScale();

	/*
	set the movementScale, this should be a value  (0.f,1.f]
	This is in what direction and scale should the field move in regards to the
	sf::vector2f displacement that you supply to StarField::move
	The default is a negative value, so the field moves opposite the movement
	*/
	void setMovementScale(float scale);

	//gets the moving starfield count
	unsigned getMovingLayerCount();

	//sets the moving starfield count, this has no effect unless generate is called, limit is MAX_MOVING_STAR_FIELDS
	void setMovingStarFieldCount(unsigned layerCount);

	/*
	Returns the total number of layers drawing.
	*/
	unsigned getTotalLayersCount();

	//sets the screen size, which updates random star spread
	// you must call generate/loadtexturefromfile to replace update the layer
	void setScreenSize(sf::Vector2f size);

	//returns if the layer has been generated or loaded
	bool isLayerReady(unsigned layer);

	//returns if the layer is set to draw
	bool isLayerEnabled(unsigned layer);

	//toggles a layers display state
	bool toggleLayerEnabled(unsigned layer);

	//enables or disables a layer from drawing
	void enableDisableLayer(unsigned layer, bool enableDisable);


private:
	//Methods
	void drawStarField(int layer);
	void initRandomGenerators();

	//====member data//====
	//stuff for random generation
	unsigned mSeed;
	default_random_engine mGenerator;
	default_random_engine mStandardGenerator;
	uniform_real_distribution<float> mDistributionX;
	uniform_real_distribution<float> mDistributionY;
	uniform_real_distribution<float> mStarSpacerDistribution;
	uniform_real_distribution<float> mStandardDistribution;

	//view info
	bool mHaveFixedView;
	sf::View mFixedView;

	sf::Vector2f mScreenSize;
	//sf::RenderWindow& mWindow;

	//star field stuff
	bool mLayerReady[MAX_MOVING_STAR_FIELDS + 1];
	bool mDrawLayer[MAX_MOVING_STAR_FIELDS + 1];
	float mMovementScale;
	unsigned mMovingFieldsCount;

	sf::CircleShape mStarShape;
	sf::Sprite mStarFieldSprites[MAX_MOVING_STAR_FIELDS + 1];
	sf::Texture mStarFieldTextures[MAX_MOVING_STAR_FIELDS + 1];
	vector<Star> mStarFields[MAX_MOVING_STAR_FIELDS + 1];
	bool mDrawMovingField[MAX_MOVING_STAR_FIELDS];
};


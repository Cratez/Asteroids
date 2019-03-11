#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Constants.h"

inline sf::Vector2f coordsWorldToPixel(b2Vec2 location)
{
	return sf::Vector2f(location.x * SCALE, location.y * SCALE);
}

inline sf::Vector2f coordsWorldToPixel(float x, float y)
{
	return sf::Vector2f(x * SCALE, y * SCALE);
}

inline b2Vec2 coordsPixelToWorld(sf::Vector2f location)
{
	return b2Vec2(location.x / SCALE, location.y / SCALE);
}

inline b2Vec2 coordsPixelToWorld(float x, float y)
{
	return b2Vec2(x / SCALE, y / SCALE);
}

inline sf::Vector2f toSFVect2f(b2Vec2 vect) {
	return sf::Vector2f(vect.x, vect.y);
}

inline b2Vec2 toB2Vect2(sf::Vector2f vect) {
	return b2Vec2(vect.x, vect.y);
}

inline float radiansToDegrees(float32 angle) {
	return angle * 180 / b2_pi;
}

inline float32 degreesToRadians(float angle) {
	return angle * b2_pi / 180;
}


#ifndef NDEBUG
#   define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif
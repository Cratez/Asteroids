#include "TextureManager.h"
#include <iostream>

using namespace std;

TextureManager::TextureManager()
{
}

bool TextureManager::loadTexture(std::string namekey, std::string texturePath)
{
	sf::Texture texture;
	bool result = texture.loadFromFile(texturePath);
	if(result)
		mTextures[namekey] = texture;

	return result;
}

const sf::Texture & TextureManager::getTextureRef(std::string namekey)
{
	return mTextures[namekey];
}

bool TextureManager::addTexture(std::string namekey, const sf::Texture & texture)
{
	if (mTextures.count(namekey) == 1)
	{
		cout << "Duplicate texture key [" << namekey << "]" << endl;
		cout << "Defalting to replace texture" << endl;
		//return false;
	}

	mTextures[namekey] = texture;
	return true;
}

bool TextureManager::hasTexture(std::string namekey)
{
	if (mTextures.count(namekey) > 0)
		return true;
	return false;
}

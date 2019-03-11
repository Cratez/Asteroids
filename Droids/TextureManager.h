#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <map>

class TextureManager
{
public:
	TextureManager();

	bool loadTexture(std::string namekey, std::string texturePath);
	const sf::Texture& getTextureRef(std::string namekey);
	bool addTexture(std::string namekey, const sf::Texture& texture);
	bool hasTexture(std::string namekey);

private:
	std::map<std::string, sf::Texture> mTextures;
};


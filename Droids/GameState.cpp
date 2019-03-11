#include "GameState.h"

GameState::GameState(Game& theGame) :
	game(theGame),
	window(theGame.getWindow()),
	finished(false)
{

}

bool GameState::isFinished()
{
	return finished;
}

TextureManager & GameState::getTextureManager()
{
	return textureManager;
}

void GameState::setFinished()
{
	finished = true;
}

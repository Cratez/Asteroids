#include "PauseState.h"


PauseState::PauseState(Game & game) : GameState(game)
{
	fixedView = window.getDefaultView();
	filterScreen.setSize(fixedView.getSize());
	filterScreen.setFillColor(sf::Color(0,0,0,128));

	//get the screen texture, this must be the window size to avoid crashes
	originalScreenTexture.create(window.getSize().x, window.getSize().y);
	originalScreenTexture.update(window);

	//the screen texture may have grown or shrunk compared to the view dimensions
	//so to draw to an old view that is say 500,500, when your new texture is 
	//potentially 100,100 or 1000,1000 due to window size presents an issue
	
	//set sprite to size of old view
	sf::Vector2f spriteScale(
		fixedView.getSize().x / window.getSize().x,
		fixedView.getSize().y / window.getSize().y);
	originalScreenSprite.setScale(spriteScale);

	originalScreenSprite.setTexture(originalScreenTexture);
}

void PauseState::handleInput()
{
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			setFinished();
		}
	}
}

void PauseState::update(const float dtSeconds)
{
}

void PauseState::draw()
{
	window.clear(sf::Color::Green);

	sf::View oldView = window.getView();
	window.setView(fixedView);
	window.draw(originalScreenSprite);
	window.draw(filterScreen);
	window.display();
	window.setView(oldView);
}

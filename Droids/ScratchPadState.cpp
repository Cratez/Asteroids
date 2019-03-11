#include "ScratchPadState.h"
#include "PauseState.h"
#include <sstream>

ScratchPadState::ScratchPadState(Game& game) :
	GameState(game),
	standardView(window.getView()),
	fixedView(window.getView()),
	starField(fixedView,3),
	world(b2Vec2(0,0)),
	ship(*this,world)
{
	test.setRadius(100);
	test.setPosition(100, 100);
	test.setOrigin(100, 100);

	shipCoordText.setPosition(100, 100);
	shipCoordText.setString("Ship coord: (NA,NA)");
	shipCoordText.setFillColor(sf::Color::Red);
	shipCoordText.setCharacterSize(50);

	sf::RenderTexture rend;
	rend.create(100, 100);
	rend.draw(shipCoordText);
	rend.display();
	sf::Texture texture = rend.getTexture();
	sf::Image img = texture.copyToImage();
	img.saveToFile("test.png");

	//starField.generate();
	starField.loadTextureFromFile(0, "starfield0.png");
	starField.loadTextureFromFile(1, "starfield1.png");
	starField.loadTextureFromFile(2, "starfield2.png");
	starField.loadTextureFromFile(3, "starfield3.png");
	starField.clearFixedView();
}

void ScratchPadState::draw()
{
	window.clear();

	window.setView(standardView);

	window.draw(starField);

	window.draw(ship);

	window.setView(fixedView);
	sf::Vector2f coords = ship.getPosition();
	stringstream sstream;
	sstream << "Ship Coord: (" << coords.x << "," << coords.y << ")";
	shipCoordText.setString(sstream.str());
	window.draw(shipCoordText);
	//window.draw(test);

	window.display();
}

void ScratchPadState::update(float dtSeconds)
{
	ship.update(dtSeconds);
	world.Step(dtSeconds, 8, 3);
	//standardView.setCenter(ship.getPosition());
	window.setView(standardView);
	starField.move(ship.getDeltaMovement());
}

void ScratchPadState::handleInput()
{
	sf::Vector2f offset = sf::Vector2f(0,0);

	auto screenMove = [&]() {
		standardView.move(offset);
		starField.move(offset);
	};

	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) 
		{
			game.pushState(new PauseState(game));
		}
		else if (event.type == sf::Event::MouseWheelMoved) // Zomm in or out if the mouse wheel moves
		{
			standardView.zoom(1.f + event.mouseWheel.delta*-0.1f);
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A))
		{
			// Move left
			offset += sf::Vector2f(-10.f, 0.f);
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D))
		{
			// Move right
			offset += sf::Vector2f(10.f, 0.f);
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W))
		{
			// Move up
			offset += sf::Vector2f(0.f, -10.f);
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S))
		{
			ship.setForceVector(sf::Vector2f(100, 100));
			// Move down
			offset += sf::Vector2f(0.f, 10.f);
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0))
		{
			starField.enableDisableLayer(0, !starField.isLayerEnabled(0));
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1))
		{
			starField.enableDisableLayer(1, !starField.isLayerEnabled(1));
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num2))
		{
			starField.enableDisableLayer(2, !starField.isLayerEnabled(2));
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num3))
		{
			starField.enableDisableLayer(3, !starField.isLayerEnabled(3));
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num4))
		{
			starField.enableDisableLayer(4, !starField.isLayerEnabled(4));
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num5))
		{
			starField.enableDisableLayer(5, !starField.isLayerEnabled(5));
		}
		else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::O))
		{
			starField.generate();
		}
	}
	//offset += sf::Vector2f(10.f, 0.f);
	screenMove();
}

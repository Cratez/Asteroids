#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

#include <Box2D/Box2D.h>
#include "Constants.h"
#include "StarField.h"

using namespace std;

#include "Game.h"
#include "ScratchPadState.h"

int main() 
{
	Game engine;
	engine.pushState(new ScratchPadState(engine));
	engine.start();
}

void CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / 30.f, Y / 30.f);
	BodyDef.type = b2_staticBody;

	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE); // Creates a box shape. Divide your desired width and height by 2.

	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;  // Sets the density of the body
	FixtureDef.shape = &Shape; // Sets the shape
	Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

void CreateBox(b2World& World, float MouseX, float MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

int oldmain()
{
	sf::RenderWindow window(sf::VideoMode(VIDEOMODE), "AsterDroids - a silly space game");
	//window.setFramerateLimit(60);
	const sf::View fixedView = window.getView();
	sf::View view = window.getView();


	//custom starfield
	//StarField starField(window,fixedView);
	StarField starField(fixedView,5);
	//starField.loadTextureFromFile(0, "starfield0.png");
	//starField.clearFixedView();
	starField.generate();
	//starField.setMovementScale(5);
	//starField.enableDisableLayer(0, false);

	//box2d setup
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);
	CreateGround(World, 960, 500.f);

	//sfml shapes for box2d
	sf::RectangleShape ground;
	ground.setSize(sf::Vector2f(800, 16));
	sf::RectangleShape box;
	box.setSize(sf::Vector2f(32,32));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color::Magenta);

	sf::Clock clock;
	sf::Clock frameRateClock;
	sf::Clock perfClock;
	unsigned frameCounter = 0;
	unsigned boxesCreated = 0;
	sf::Time drawTime = sf::seconds(0);
	sf::Time physicsTime = sf::seconds(0);

	while (window.isOpen())
	{
		sf::Event event;
		sf::Vector2f offset;

		//if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		//	sf::Vector2f point = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		//	CreateBox(World, point.x, point.y);
		//	boxesCreated++;
		//}

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::MouseButtonPressed) 
			{
				sf::Vector2f point = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				CreateBox(World, point.x, point.y);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			{
 				//starField.DrawMovingStars = !starField.DrawMovingStars;
				/*for (int i = 1; i < starField.getMovingStarFieldCount()+1; i++) {
					starField.toggleLayerEnabled(i);
				}*/
				starField.toggleLayerEnabled(1);
			}
			else if (event.type == sf::Event::MouseWheelMoved) // Zomm in or out if the mouse wheel moves
			{
				view.zoom(1.f + event.mouseWheel.delta*-0.1f);
			}

			if (event.key.code == sf::Keyboard::R) // Rotate counter clockwise
			{
				view.rotate(0.25f);
			}
			else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::T) // Rotate clockwise
			{

				view.rotate(-0.25f);
			}

			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A))
			{
				// Move left
				offset = sf::Vector2f(-10.f, 0.f);
				view.move(offset);
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D))
			{
				// Move right
				offset = sf::Vector2f(10.f, 0.f);
				view.move(offset);
				
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W))
			{
				// Move up
				offset = sf::Vector2f(0.f, -10.f);
				view.move(offset);
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S))
			{
				// Move down
				offset = sf::Vector2f(0.f, 10.f);
				view.move(offset);
			}
			else if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num0))
			{
				starField.enableDisableLayer(0,!starField.isLayerEnabled(0));
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

		/** Simulate the world */
		perfClock.restart();
		World.Step(clock.restart().asSeconds(), 8, 3);
		physicsTime += perfClock.restart();

		starField.move(offset);

		//clear the window
		window.clear();

		//update the view position
		window.setView(view);

		//drawing time
		//starField.Draw(window);
		window.draw(starField);
		
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				box.setOrigin(16.f, 16.f);
				box.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				box.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				window.draw(box);
			}
			else
			{
				
				ground.setOrigin(400.f, 8.f);
				ground.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				ground.setRotation(BodyIterator->GetAngle() * 180 / b2_pi );
				window.draw(ground);
			}
		}

		/*const float BIG_G = 6.67;
		for (b2Body* b1 = World.GetBodyList(); b1; b1 = b1->GetNext())
		{
			if (b1->GetType() != b2_dynamicBody)
			{
				continue;
			}
			b2Vec2 pi = b1->GetWorldCenter();
			float mi = b1->GetMass();
			for (b2Body* b2 = World.GetBodyList(); b2; b2 = b2->GetNext())
			{
				if (b1 == b2 || b2->GetType() != b2_dynamicBody)
				{				
					continue;
				}
				b2Vec2 pk = b2->GetWorldCenter();
				float mk = b2->GetMass();
				b2Vec2 delta = pk - pi;
				float r = delta.Length();
				float force = BIG_G * mi * mk / (r*r);
				delta.Normalize();
				b1->ApplyForce(force * delta, pi,false);
				b2->ApplyForce(-force * delta, pk,false);
			}
		}*/

		//render the frame
		window.display();
		drawTime += perfClock.restart();
		frameCounter++;

		if (frameRateClock.getElapsedTime().asSeconds() > 10) {
			sf::Time elapsed = frameRateClock.restart();

			//grab time
			cout << "Frames rendered: " << frameCounter << endl;
			cout << "Time elapses: " << elapsed.asSeconds() << " seconds" << endl;
			cout << "DrawTime: " << drawTime.asSeconds() << " seconds" << endl;
			cout << "PhysicsTime: " << physicsTime.asSeconds() << " seconds" << endl;
			cout << "Framerate: " << frameCounter / elapsed.asSeconds() << endl;
			cout << "Boxes created: " << boxesCreated << endl << endl;
			frameCounter = 0;
			drawTime = sf::seconds(0);
			physicsTime = sf::seconds(0);
		}
	}

	return 0;
}

// SFML header file for graphics, there are also ones for Audio, Window, System and Network
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include "Framework.h"

extern int gScreenWidth;
extern int gScreenHeight;

sf::RenderWindow gWindow(sf::VideoMode(gScreenWidth, gScreenHeight), "C++ Framework : Maze");
sf::CircleShape gCircleShape(20, 64);
sf::RectangleShape gRectangleShape({ 40,30 });

//extern int x;
//extern int y;

extern int gTimeDelayMS;

using namespace std::this_thread;
using namespace std::chrono;

EKeyPressed lastKeyPressed{ EKeyPressed::eNone };
sf::Clock gClock;

bool UpdateFramework()
{
	static bool init{ true };

    if (!gWindow.isOpen())
      return false;

	if (init)
	{
		gCircleShape.setFillColor(sf::Color::Red);
		init = false;
	}

    // Handle any pending SFML events
    // These cover keyboard, mouse,joystick etc.	
    sf::Event event;
    while (gWindow.pollEvent(event))
    {
      switch(event.type)
      {
        case sf::Event::Closed:
          gWindow.close();
        break;	
        default:
          break;
      }
    }

	lastKeyPressed = EKeyPressed::eNone;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		lastKeyPressed = EKeyPressed::eLeft;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		lastKeyPressed = EKeyPressed::eRight;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		lastKeyPressed = EKeyPressed::eUp;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		lastKeyPressed = EKeyPressed::eDown;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		lastKeyPressed = EKeyPressed::eSave;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
		lastKeyPressed = EKeyPressed::eLoad;

    // Get the window to display its contents
    gWindow.display();

    sleep_for(milliseconds(gTimeDelayMS));

    // We must clear the winint gTimeDelayMS;dow each time round the loop
    gWindow.clear();

    // draw our circle shape to the window
   // gWindow.draw(gRedCircleShape);
   
	// Draw a grid TODO

    return true;
}

void ChangeColour(unsigned char red, unsigned char green, 
	unsigned char blue, unsigned char alpha)
{
	gCircleShape.setFillColor({ red,green,blue,alpha });
	gRectangleShape.setFillColor({ red,green,blue,alpha });
}

void DrawCircle(int x,int y, int radius)
{
    gCircleShape.setPosition((float)x,(float)y);
    gCircleShape.setRadius((float)radius);

    // draw our circle shape to the window
    gWindow.draw(gCircleShape);
}

void DrawRectangle(int x, int y, int width, int height)
{
	gRectangleShape.setSize({ (float)width,(float)height });
	gRectangleShape.setPosition((float)x, (float)y);
	gWindow.draw(gRectangleShape);
}

EKeyPressed GetLastKeyPressed()
{
	EKeyPressed temp{ lastKeyPressed };
	lastKeyPressed = EKeyPressed::eNone;
	return temp;
}

void StartClock()
{
	gClock.restart();
}

float GetElapsedTime()
{
	return gClock.getElapsedTime().asSeconds();
}

void GetMousePosition(int& x, int& y)
{
	sf::Vector2i position = sf::Mouse::getPosition(gWindow);

	x = position.x;
	y = position.y;
}

bool IsButtonPressed(EButton whichButton)
{
	switch (whichButton)
	{
	case EButton::eLeft:
		return sf::Mouse::isButtonPressed(sf::Mouse::Left);
	case EButton::eRight:
		return sf::Mouse::isButtonPressed(sf::Mouse::Right);
	case EButton::eMiddle:
		return sf::Mouse::isButtonPressed(sf::Mouse::Middle);
	default:
		std::cerr << "unknown button" << std::endl;
		break;
	}

	return false;
}

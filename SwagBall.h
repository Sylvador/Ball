#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>

enum SwagBallTypes
{
	DEFAULT = 0,
	DAMAGING,
	HEALING,
	NROFTYPES
};

class SwagBall
{
private:
	sf::CircleShape shape;
	int type;

	void initShape(const sf::RenderWindow& window);
public:
	SwagBall(const sf::RenderWindow& window, int type);
	virtual ~SwagBall();

	//Accessors
	const sf::CircleShape getShape() const;
	const int& getType() const;
	const float getSize() const;

	//Functions
	void update();
	void render(sf::RenderTarget& target);
};


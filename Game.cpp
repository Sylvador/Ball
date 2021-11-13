#include "Game.h"

void Game::initVariables()
{
	this->endGame = false;
	this->spawnTimerMax = 10.f;
	this->spawnTimer = this->spawnTimerMax;
	this->maxSwagBalls = 10;
	this->points = 0;
}

void Game::initFonts()
{
	if (!this->font.loadFromFile("Fonts/PottaOne-Regular.ttf"))
		throw "pizdec";
}

void Game::initText()
{
	this->guiText.setFont(this->font);
	this->guiText.setFillColor(sf::Color(255, 223, 0, 200));
	this->guiText.setCharacterSize(15);

	//End game text
	this->endGameText.setFont(this->font);
	this->endGameText.setFillColor(sf::Color(255, 0, 0, 255));
	this->endGameText.setCharacterSize(60);
	this->endGameText.setString("YOU DIED");
	this->endGameText.setOrigin(this->endGameText.getGlobalBounds().width / 2, this->endGameText.getGlobalBounds().height / 2);
	this->endGameText.setPosition(this->window->getSize().x / 2, this->window->getSize().y / 2);
}

void Game::initWindow()
{
	this->videoMode = sf::VideoMode(800, 600);
	this->window = new sf::RenderWindow(this->videoMode, "Ball", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(60);
}

//Constructors and Destructors
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initFonts();
	this->initText();
}

Game::~Game()
{
	delete this->window;
}

const bool& Game::getEndGame() const
{
	return this->endGame;
}

//Functions
const bool Game::running() const
{
	return this->window->isOpen()/* && !this->endGame*/;
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->sfmlEvent))
	{
		switch (this->sfmlEvent.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->sfmlEvent.key.code == sf::Keyboard::Escape)
				this->window->close();
		default:
			break;
		}
	}
}

void Game::spawnSwagBalls()
{
	//Timer
	if (this->spawnTimer < this->spawnTimerMax)
		this->spawnTimer += 1.f;
	else
	{
		if (this->swagBalls.size() < this->maxSwagBalls)
		{
			this->swagBalls.push_back(SwagBall(*this->window, this->randBallType()));
			this->spawnTimer = 0.f;
		}
	}
}

const int Game::randBallType() const
{
	int type = SwagBallTypes::DEFAULT;
	int randValue = rand() % 100 + 1;

	if (randValue > 60 && randValue <= 80)
		type = DAMAGING;
	else if (randValue > 80 && randValue <= 100)
		type = HEALING;

	return type;
}

void Game::updatePlayer()
{
	this->player.update(this->window);

	if (this->player.getHp() <= 0)
		this->endGame = true;
}

void Game::updateCollision()
{
	//Check the collision
	for (size_t i = 0; i < this->swagBalls.size(); i++)
	{
		if (this->player.getShape().getGlobalBounds().intersects(this->swagBalls[i].getShape().getGlobalBounds()))
		{
			switch (this->swagBalls[i].getType())
			{
			case SwagBallTypes::DEFAULT:
				this->points++;
				this->player.getBigger(this->swagBalls[i].getSize());
				break;
			case SwagBallTypes::DAMAGING:
				this->player.takeDamage(3);
				this->player.getSmaller(this->swagBalls[i].getSize());
				break;
			case SwagBallTypes::HEALING:
				this->player.gainHealth(1);
				break;
			default:
				break;
			}

			//Remove ball
			this->swagBalls.erase(this->swagBalls.begin() + i);
		}
	}
}

void Game::updateGui()
{
	std::stringstream ss;
	ss << "Your Points: " << this->points
		<< "\nYour Health: " << this->player.getHp() << " / " << this->player.getHpMax();
	this->guiText.setString(ss.str());
}

void Game::update()
{
	this->pollEvents();

	if (!this->endGame)
	{
		this->updatePlayer();
		this->spawnSwagBalls();
		this->updateCollision();
		this->updateGui();
	}
}

void Game::renderGui(sf::RenderTarget* target)
{
	target->draw(this->guiText);
}

void Game::render()
{
	this->window->clear();

	//Render stuff
	this->player.render(this->window);

	for (auto i : this->swagBalls)
	{
		i.render(*this->window);
	}

	//Render gui
	this->renderGui(this->window);

	if (this->endGame)
		this->window->draw(this->endGameText);

	this->window->display();
}

//Functions
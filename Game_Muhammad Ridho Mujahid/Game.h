#pragma once

#include<iostream>
#include<vector>
#include<ctime>
#include<sstream>
#include"Player.h"
#include"Enemy.h"
#include"Bullet.h"

#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>
#include<SFML/Network.hpp>

/*
	Class yang berfungsi sebagai game engine.
*/

class Game
{
private:

	//Window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//peluru
	std::map<std::string, sf::Texture*> textures;
	std::vector<Bullet*> bullets;

	//Latar
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;
	//sf::Music music;
	
	//Resources
	sf::Font font;

	//Text
	sf::Text uiText;

	//Game Logic
	bool endGame;
	unsigned points;
	int nyawa;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;

	//Player
	Player* player;

	//Game Object
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	//Private functions
	void initWorld();
	void initVariables();
	void initWindow();
	void initTextures(); //bullet
	void initFonts();
	void initText();
	void initEnemies();
	void initPlayer();
public:
	//Constructor / Destructors
	Game();
	virtual ~Game();

	//Accessors
	const bool running() const;
	const bool getEndGame() const;

	//Functions
	void spawnEnemy();
	void pollEvents();
	void updateInput();
	void updateBullets();
	void updateText();
	void updateEnemies();
	void update();

	void renderWorld();
	void renderText(sf::RenderTarget& target);
	void renderEnemies(sf::RenderTarget& target);
	void renderPlayer(sf::RenderTarget& target);
	void render();

};

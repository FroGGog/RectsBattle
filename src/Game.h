#include "SFML/Graphics.hpp"


#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>


#include "EntityManager.h"

class Game {

private:

	sf::RenderWindow m_window;
	sf::Event evt;

	EntityManager m_entities;

	bool m_paused;
	bool m_running;
	bool manualSpawn;

	//start|stop systems
	bool m_collisions;
	bool m_spawner;
	bool m_movement;

	int score;
	float spawnRate;

	sf::Clock clock;
	sf::Clock deltaClock;
	sf::Time enemySpawner;

	sf::Font font_main;
	sf::Text text_score;

	void init();
	void initPlayer();
	void initBackground();

	void sWindowEvents();
	void sRender();
	void sSpawnEnimes();
	void sSpawnAfterKill(int fragments, Vec2<float> pos_, const sf::Color color_);
	void sMovement();
	void sRotating();
	void sInput();
	void sWinCollision();
	void sCollision();
	void sShooting();
	void sLifeSpan();
	void sUpdateScore();
	void sImGUI();

	void restart();

public:

	Game();

	void update();

	bool isRunning() const;
};
#include "SFML/Graphics.hpp"

#include "EntityManager.h"

class Game {

private:

	sf::RenderWindow m_window;
	sf::Event evt;

	EntityManager m_entities;

	bool m_paused;
	bool m_running;

	int score;

	sf::Clock clock;
	sf::Time enemySpawner;

	void init();
	void initPlayer();

	void sWindowEvents();
	void sRender();
	void sSpawnEnimes();
	void sMovement();
	void sInput();
	void sCollision();
	void sShooting();

	void restart();

public:

	Game();

	void update();

	bool isRunning() const;
};
#include "SFML/Graphics.hpp"

#include "EntityManager.h"

class Game {

private:

	sf::RenderWindow m_window;
	sf::Event evt;

	EntityManager m_entities;

	bool m_paused;
	bool m_running;

	sf::Clock clock;
	sf::Time enemySpawner;

	void init();

	void sWindowEvents();
	void sRender();
	void sSpawnEnimes();
	void sMovement();
	void sCollision();


public:

	Game();

	void update();

	bool isRunning() const;
};
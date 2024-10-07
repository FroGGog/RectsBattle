#include "SFML/Graphics.hpp"

#include "EntityManager.h"

class Game {

private:

	sf::RenderWindow m_window;
	sf::Event evt;

	EntityManager m_entities;

	bool m_paused;
	bool m_running;

	void init();

	void sWindowEvents();
	void sRender();
	void sSpawnEnimes();
	void sMovement();


public:

	Game();

	void update();

	bool isRunning() const;
};
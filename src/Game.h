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
	void sInput();
	void sCollision();
	void sShooting();
	void sLifeSpan();
	void sUpdateScore();

	void restart();

public:

	Game();

	void update();

	bool isRunning() const;
};
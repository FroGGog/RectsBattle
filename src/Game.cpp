#include "Game.h"

Game::Game()
{
	init();

	enemySpawner = clock.getElapsedTime();

}

void Game::update()
{
	sWindowEvents();

	m_entities.deleteEntities();
	m_entities.update();

	sSpawnEnimes();
	sMovement();
	sCollision();

	sRender();
}

bool Game::isRunning() const
{
	return m_running;
}

void Game::init()
{
	m_window.create(sf::VideoMode{ 1280, 720 }, "Assigment2", sf::Style::Close);
	m_window.setFramerateLimit(60);
}

void Game::sWindowEvents()
{
	while (m_window.pollEvent(evt)) {

		switch (evt.type)
		{
		case sf::Event::Closed:
			m_running = false;
			break;
		default:
			break;
		}
	}
}

void Game::sRender()
{
	m_window.clear();

	for (auto e : m_entities.getEntities("Enemy")) {
		m_window.draw(e->cShape->shape);
	}

	m_window.display();

}

void Game::sSpawnEnimes()
{
	enemySpawner = clock.getElapsedTime();

	if (m_entities.getEntities().size() < 15 && enemySpawner.asSeconds() > .5f) {

		auto e = m_entities.addEntity("Enemy");

		e->cTransform = std::make_shared<CTransform>(Vec2<float>{(float)(rand() % (m_window.getSize().x - 250) + 100), (float)(rand() % (m_window.getSize().y - 250) + 100)},
			Vec2<float>{(float)(rand() % 10), (float)(rand() % 10)}, Vec2<float>{1, 1}, 5.f);
		sf::Color color{ (sf::Uint8)(rand() % 255) , (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255) , 255};
		e->cShape = std::make_shared<CShape>((float)((rand() % 50) + 15), (float)((rand() % 16) + 3), e->cTransform->pos, color, sf::Color::White, 2.f);

		e->cLifeSpan = std::make_shared<CLifespan>((float)((rand() % 15) + 1));

		enemySpawner = clock.restart();

	}
	
}

void Game::sMovement()
{
	// move objects 
	for (auto e : m_entities.getEntities("Enemy")) {

		e->cShape->shape.move(e->cTransform->speed.getCords().first, e->cTransform->speed.getCords().second);
		e->cTransform->pos = Vec2{ e->cShape->shape.getPosition().x, e->cShape->shape.getPosition().y };

	}

	//rotate objects
	for (auto e : m_entities.getEntities("Enemy")) {

		e->cShape->shape.rotate(e->cTransform->rotation);

	}

	//minus life span TODO get this to new system
	for (auto e : m_entities.getEntities("Enemy")) {

		e->cLifeSpan->elapsedTime = e->cLifeSpan->timer.getElapsedTime();

		if (e->cLifeSpan->elapsedTime.asSeconds() > e->cLifeSpan->totalTime) {
			e->destroy();
		}

	}
}

void Game::sCollision()
{

	for (auto e : m_entities.getEntities("Enemy")) {

		if (e->cTransform->pos.getCords().first - e->cShape->shape.getGlobalBounds().width / 2 < 0) {
			e->cTransform->speed.reverse(true, false);
		}
		else if (e->cTransform->pos.getCords().first + e->cShape->shape.getGlobalBounds().width / 2 >
			m_window.getSize().x)
		{
			e->cTransform->speed.reverse(true, false);
		}


		if (e->cTransform->pos.getCords().second + e->cShape->shape.getGlobalBounds().height / 2 >
			m_window.getSize().y && e->cTransform->speed.getCords().second > 0) {
			e->cTransform->speed.reverse(false, true);
		}
		else if (e->cTransform->pos.getCords().second - e->cShape->shape.getGlobalBounds().height / 2 < 0) {
			e->cTransform->speed.reverse(false, true);
		}
	}

}



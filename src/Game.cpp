#include "Game.h"

Game::Game()
{
	init();
}

void Game::update()
{
	m_entities.update();

	sSpawnEnimes();
	sMovement();

	sWindowEvents();

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
	if (m_entities.getEntities().size() < 2) {

		auto e = m_entities.addEntity("Enemy");

		e->cTransform = std::make_shared<CTransform>(Vec2<float>{150.f, 200.f}, Vec2<float>{2.f, 3.f}, Vec2<float>{1, 1}, 0);
		e->cShape = std::make_shared<CShape>(50.f, 3, e->cTransform->pos, sf::Color::Red, sf::Color::White, 1.f);

	}
	
}

void Game::sMovement()
{
	for (auto e : m_entities.getEntities("Enemy")) {

		e->cShape->shape.move(e->cTransform->speed.getCords().first, e->cTransform->speed.getCords().first);

	}
}



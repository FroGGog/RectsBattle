#include "Game.h"

Game::Game()
{
	init();

	initPlayer();

}

void Game::update()
{
	sWindowEvents();

	m_entities.deleteEntities();
	m_entities.update();

	sSpawnEnimes();
	sInput();
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

void Game::initPlayer()
{

	auto e = m_entities.addEntity("Player");

	e->cShape = std::make_shared<CShape>(30.f, 9, Vec2<float>{(float)(m_window.getSize().x / 2),(float)(m_window.getSize().y / 2)},
		sf::Color::Red, sf::Color::Cyan, 3.f);
	e->cTransform = std::make_shared<CTransform>(Vec2<float>{e->cShape->shape.getPosition().x, e->cShape->shape.getPosition().y},
		Vec2<float>{5.f, 5.f}, Vec2<float>{1, 1}, 5.f);
	e->cInput = std::make_shared<CInput>();
	e->cShooting = std::make_shared<CShooting>(2.f);
	

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

	for (auto& e : m_entities.getEntities("Enemy")) {

		m_window.draw(e->cShape->shape);
	}

	for (auto& e : m_entities.getEntities("Player")) {

		m_window.draw(e->cShape->shape);

	}

	m_window.display();

}

void Game::sSpawnEnimes()
{
	enemySpawner = clock.getElapsedTime();

	if (m_entities.getEntities().size() < 15 && enemySpawner.asSeconds() > 0.f) {

		auto e = m_entities.addEntity("Enemy");

		e->cTransform = std::make_shared<CTransform>(Vec2<float>{(float)(rand() % (m_window.getSize().x - 250) + 100), (float)(rand() % (m_window.getSize().y - 250) + 100)},
			Vec2<float>{(float)((rand() % 8) + 2), (float)((rand() % 8) + 2)}, Vec2<float>{1, 1}, 5.f);
		sf::Color color{ (sf::Uint8)(rand() % 255) , (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255) , 255};
		e->cShape = std::make_shared<CShape>((float)((rand() % 35) + 15), (float)((rand() % 16) + 3),
			e->cTransform->pos, color, sf::Color::White, (float)((rand() % 5) + 2));

		e->cLifeSpan = std::make_shared<CLifespan>((float)((rand() % 20) + 5));

		enemySpawner = clock.restart();

	}
	
}

void Game::sMovement()
{
	// move objects 
	for (auto& e : m_entities.getEntities("Enemy")) {

		e->cShape->shape.move(e->cTransform->speed.getCords().first, e->cTransform->speed.getCords().second);
		e->cTransform->pos = Vec2{ e->cShape->shape.getPosition().x, e->cShape->shape.getPosition().y };

	}

	for (auto& e : m_entities.getEntities("Player")) {

		if (e->cInput->RIGHT) {
			e->cShape->shape.move(e->cTransform->speed.getCords().first, 0);
		}
		else if (e->cInput->LEFT) {
			e->cShape->shape.move(e->cTransform->speed.getCords().first * -1.f, 0);
		}

		if (e->cInput->UP) {
			e->cShape->shape.move(0, e->cTransform->speed.getCords().second * -1.f);
		}
		else if (e->cInput->DOWN) {
			e->cShape->shape.move(0, e->cTransform->speed.getCords().second * 1.f);
		}

		e->cTransform->pos = Vec2{ e->cShape->shape.getPosition().x, e->cShape->shape.getPosition().y };
	}

	//rotate objects
	for (auto& e : m_entities.getEntities()) {

		e->cShape->shape.rotate(e->cTransform->rotation);

	}



	//minus life span TODO get this to new system
	for (auto& e : m_entities.getEntities("Enemy")) {

		e->cLifeSpan->elapsedTime = e->cLifeSpan->timer.getElapsedTime();

		if (e->cLifeSpan->elapsedTime.asSeconds() > e->cLifeSpan->totalTime) {
			e->destroy();
		}

	}
}

void Game::sInput()
{
	for (auto& player : m_entities.getEntities("Player"))
	{
		// TODO implent diagonal movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			player->cInput->LEFT = true;
		}
		else {
			player->cInput->LEFT = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player->cInput->RIGHT = true;
		}
		else {
			player->cInput->RIGHT = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player->cInput->UP = true;
		}
		else {
			player->cInput->UP = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player->cInput->DOWN = true;
		}
		else {
			player->cInput->DOWN = false;
		}

		// Shooting system
		if(player->cShooting->elapsedTime.asSeconds() < player->cShooting->reloading)
		{
			player->cShooting->elapsedTime = player->cShooting->timer.getElapsedTime();
		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			if (player->cShooting->elapsedTime.asSeconds() >
				player->cShooting->reloading) {

				player->cShooting->elapsedTime = player->cShooting->timer.restart();

				// TODO get mouse position

				//Spawn projectile
				auto bullet = m_entities.addEntity("Bullet");
				// TODO add all componets for bullet

			}

		}
	}

}

void Game::sCollision()
{

	for (auto& e : m_entities.getEntities("Enemy")) {

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


	for (auto& player : m_entities.getEntities("Player")) {

		if (player->cTransform->pos.getCords().first - player->cShape->shape.getGlobalBounds().width / 2 < 0) {
			player->cShape->shape.setPosition(player->cShape->shape.getGlobalBounds().width / 2,
				player->cShape->shape.getPosition().y);
		}
		else if (player->cTransform->pos.getCords().first + player->cShape->shape.getGlobalBounds().width / 2 > m_window.getSize().x) {
			player->cShape->shape.setPosition(m_window.getSize().x - player->cShape->shape.getGlobalBounds().width / 2,
				player->cShape->shape.getPosition().y);
		}

		if (player->cTransform->pos.getCords().second - player->cShape->shape.getGlobalBounds().height / 2 < 0) {
			player->cShape->shape.setPosition(player->cShape->shape.getPosition().x,
				player->cShape->shape.getGlobalBounds().height / 2);
		}
		else if (player->cTransform->pos.getCords().second + player->cShape->shape.getGlobalBounds().height / 2 > m_window.getSize().y) {
			player->cShape->shape.setPosition(player->cShape->shape.getPosition().x,
				m_window.getSize().y - player->cShape->shape.getGlobalBounds().height / 2);
		}

	}

}



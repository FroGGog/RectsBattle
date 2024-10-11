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
	sLifeSpan();
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

	score = 0;
	clock.restart();
}

void Game::initPlayer()
{

	auto e = m_entities.addEntity("Player");

	e->cShape = std::make_shared<CShape>(30.f, 9, Vec2<float>{(float)(m_window.getSize().x / 2),(float)(m_window.getSize().y / 2)},
		sf::Color::Red, sf::Color::Cyan, 3.f);
	e->cTransform = std::make_shared<CTransform>(Vec2<float>{e->cShape->shape.getPosition().x, e->cShape->shape.getPosition().y},
		Vec2<float>{5.f, 5.f}, Vec2<float>{1, 1}, 5.f);
	e->cInput = std::make_shared<CInput>();
	e->cShooting = std::make_shared<CShooting>(1.f);
	

}

void Game::sWindowEvents()
{
	while (m_window.pollEvent(evt)) {

		switch (evt.type)
		{
		case sf::Event::Closed:
			m_running = false;
			break;
		case sf::Event::MouseButtonPressed:
			if (evt.key.code == sf::Mouse::Left) {
				sShooting();
			}
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

	for (auto& e : m_entities.getEntities("Bullet")) {

		m_window.draw(e->cShape->shape);

	}

	m_window.display();

}

void Game::sSpawnEnimes()
{
	enemySpawner = clock.getElapsedTime();

	if (m_entities.getEntities("Enemy").size() < 1 && enemySpawner.asSeconds() > 1.5f) {

		auto e = m_entities.addEntity("Enemy");

		e->cTransform = std::make_shared<CTransform>(Vec2<float>{(float)(rand() % (m_window.getSize().x - 250) + 100), (float)(rand() % (m_window.getSize().y - 250) + 100)},
			Vec2<float>{(float)((rand() % 8) + 2), (float)((rand() % 8) + 2)}, Vec2<float>{1, 1}, 5.f);
		//prevent enemy from spawning inside player
		while ((m_entities.getEntities("Player")[0]->cTransform->pos - e->cTransform->pos).lenght() < 100.f) {
			e->cTransform->pos = Vec2<float>{ (float)(rand() % (m_window.getSize().x - 250) + 100), (float)(rand() % (m_window.getSize().y - 250) + 100) };
		}


		sf::Color color{ (sf::Uint8)(rand() % 255) , (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255) , 255};
		e->cShape = std::make_shared<CShape>((float)((rand() % 35) + 15), (float)((rand() % 16) + 3),
			e->cTransform->pos, color, sf::Color::White, 3.f);

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

	for (auto& e : m_entities.getEntities("Bullet")) {

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
		
	}

}

void Game::sCollision()
{
	//check border collision
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

	for (auto& bullet : m_entities.getEntities("Bullet")) {

		if (bullet->cTransform->pos.getCords().first > m_window.getSize().x || bullet->cTransform->pos.getCords().first < 0) {
			bullet->destroy();
		}

		else if (bullet->cTransform->pos.getCords().second > m_window.getSize().y || bullet->cTransform->pos.getCords().second < 0) {
			bullet->destroy();
		}

	}

	// check box collision
	for (auto& bullet : m_entities.getEntities("Bullet")) {

		for (auto& enemy : m_entities.getEntities("Enemy")) {

			if (bullet->cShape->shape.getGlobalBounds().intersects(enemy->cShape->shape.getGlobalBounds())) {

				bullet->destroy();
				enemy->destroy();
			}

		}
	}
	for (auto& player : m_entities.getEntities("Player")) {

		for (auto& enemy : m_entities.getEntities("Enemy")) {

			if (player->cShape->shape.getGlobalBounds().intersects(enemy->cShape->shape.getGlobalBounds())) {

				enemy->destroy();
				player->destroy();
				restart();

			}

		}

	}



}

void Game::sShooting()
{
	if (m_entities.getEntities("Bullet").size() >= 1) {
		return;
	}

	for (auto& player : m_entities.getEntities("Player")) {

		// Shooting system
		if (player->cShooting->elapsedTime.asSeconds() < player->cShooting->reloading)
		{
			player->cShooting->elapsedTime = player->cShooting->timer.getElapsedTime();
			if (player->cShooting->elapsedTime.asSeconds() > player->cShooting->reloading) {
				player->cShooting->canShoot = true;
			}
		}
		else {
			player->cShooting->canShoot = true;
		}

		if (player->cShooting->canShoot) {

			player->cShooting->elapsedTime = player->cShooting->timer.restart();

			sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
			Vec2<float> mouseVec{ (float)mousePos.x, (float)mousePos.y };

			//Spawn projectile
			auto bullet = m_entities.addEntity("Bullet");
			bullet->cTransform = std::make_shared<CTransform>(Vec2<float>{ player->cTransform->pos }, Vec2<float>{ 0, 0 }, Vec2<float>{ 1, 1 }, 0);

			Vec2<float> dirVec = mouseVec - bullet->cTransform->pos;

			float angle = atan2f(dirVec.getCords().first, dirVec.getCords().second);

			bullet->cTransform->speed = Vec2<float>{ sin(angle) * 17.f, cos(angle) * 17.f };

			bullet->cShape = std::make_shared<CShape>(15.f, 16, Vec2<float>{ bullet->cTransform->pos }, sf::Color::White, sf::Color::White, 0.f);
			bullet->cLifeSpan = std::make_shared<CLifespan>(1.f);

			player->cShooting->canShoot = false;


		}

	}


}

void Game::sLifeSpan()
{

	//minus lifespan
	for (auto& e : m_entities.getEntities()) {

		if (e->cLifeSpan == nullptr) {
			continue;
		}

		e->cLifeSpan->elapsedTime = e->cLifeSpan->timer.getElapsedTime();
		float procent =  1 - (((100 * e->cLifeSpan->elapsedTime.asSeconds()) / e->cLifeSpan->totalTime)) / 100;
		
		
		sf::Color newColor{ e->cShape->shape.getFillColor() };

		newColor.a = (sf::Uint8)(255 * procent);
		float outThick = 3.f * procent;

		e->cShape->shape.setFillColor(newColor);
		e->cShape->shape.setOutlineThickness(outThick);

		if (e->cLifeSpan->elapsedTime.asSeconds() > e->cLifeSpan->totalTime) {
			e->destroy();
		}

	}


}

void Game::restart()
{
	m_window.close();
	m_entities.restart();

	init();
	initPlayer();
	

}



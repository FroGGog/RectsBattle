#include "Game.h"



Game::Game()
{
	init();
	initPlayer();
	initBackground();

	ImGui::SFML::Init(m_window);

	ImGui::GetStyle().ScaleAllSizes(1.f);

	if (!font_main.loadFromFile("BrownieStencil-8O8MJ.ttf")) {
		std::cout << "ERROR::LOADFROMFILE::FONT font_main\n";
	}
	else {
		text_score.setFont(font_main);
		text_score.setCharacterSize(32);
		text_score.setPosition(0.f, 0.f);
		text_score.setString(std::to_string(score));
	}
}

void Game::update()
{
	ImGui::SFML::Update(m_window, deltaClock.restart());

	m_entities.deleteEntities();
	m_entities.update();

	sWindowEvents();

	sInput();
	sMovement();
	if (m_collisions) {
		sCollision();
	}

	sSpawnEnimes();
	sLifeSpan();
	
	sUpdateScore();
	sImGUI();

	sRender();
}

bool Game::isRunning() const
{
	return m_running;
}

void Game::init()
{
	m_window.create(sf::VideoMode{ 1280, 768 }, "Assigment2", sf::Style::Close);
	m_window.setFramerateLimit(60);

	score = 0;
	m_collisions = true;

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
	e->cShooting = std::make_shared<CShooting>(0.5f);
	

}

void Game::initBackground()
{
	bool grey = true;

	for (int collumns{ 0 }; collumns < (m_window.getSize().x / 32) + 1 ; collumns++) {

		for (int rows{ 0 }; rows < (m_window.getSize().y / 32) + 1 ; rows++) {

			auto e = m_entities.addEntity("Background");
			sf::Color color;
			if (grey) {
				color = sf::Color{ 128, 133, 136, 150 };
				grey = !grey;
			}
			else {
				color = sf::Color{ 72, 73, 75, 150 };
				grey = !grey;
			}
			//changed to vertex because of framedrops
			
			e->cVertexArray = std::make_shared<CVertex>(color);

			e->cVertexArray->vertex[0].position = sf::Vector2f{ (float)(32 * collumns), (float)(32 * rows) };
			e->cVertexArray->vertex[1].position = sf::Vector2f{ (float)(32 * collumns), (float)(32 * (rows + 1)) };
			e->cVertexArray->vertex[2].position = sf::Vector2f{ (float)(32 * (collumns + 1)), (float)(32 * (rows + 1)) };
			e->cVertexArray->vertex[3].position = sf::Vector2f{ (float)(32 * (collumns + 1)), (float)(32 * rows) };

		}

	}


}

void Game::sWindowEvents()
{
	while (m_window.pollEvent(evt)) {

		ImGui::SFML::ProcessEvent(evt);

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

	for (auto& e : m_entities.getEntities("Background")) {

		m_window.draw(e->cVertexArray->vertex);

	}

	for (auto& e : m_entities.getEntities("Enemy")) {

		m_window.draw(e->cShape->shape);
	}

	for (auto& e : m_entities.getEntities("SmallEnemy")) {

		m_window.draw(e->cShape->shape);
	}

	for (auto& e : m_entities.getEntities("Player")) {

		m_window.draw(e->cShape->shape);

	}

	for (auto& e : m_entities.getEntities("Bullet")) {

		m_window.draw(e->cShape->shape);

	}

	// GUI
	m_window.draw(text_score);

	ImGui::ShowDemoWindow();
	ImGui::SFML::Render(m_window);

	m_window.display();

}

void Game::sSpawnEnimes()
{
	enemySpawner = clock.getElapsedTime();

	if (m_entities.getEntities("Enemy").size() < 7 && enemySpawner.asSeconds() > 1.5f) {

		auto e = m_entities.addEntity("Enemy");

		e->cTransform = std::make_shared<CTransform>(Vec2<float>{(float)(rand() % (m_window.getSize().x - 250) + 100), (float)(rand() % (m_window.getSize().y - 250) + 100)},
			Vec2<float>{(float)((rand() % 8) + 2), (float)((rand() % 8) + 2)}, Vec2<float>{1, 1}, 5.f);
		//prevent enemy from spawning inside player
		while ((m_entities.getEntities("Player")[0]->cTransform->pos - e->cTransform->pos).lenght() < 100.f) {
			e->cTransform->pos = Vec2<float>{ (float)(rand() % (m_window.getSize().x - 250) + 100), (float)(rand() % (m_window.getSize().y - 250) + 100) };
		}


		sf::Color color{ (sf::Uint8)(rand() % 255) , (sf::Uint8)(rand() % 255), (sf::Uint8)(rand() % 255) , 255};
		e->cShape = std::make_shared<CShape>((float)((rand() % 35) + 15), (float)((rand() % 8) + 3),
			e->cTransform->pos, color, sf::Color::White, 3.f);

		e->cLifeSpan = std::make_shared<CLifespan>((float)((rand() % 20) + 5));
		e->cScore = std::make_shared<CScore>(e->cShape->shape.getPointCount() * 120);

		enemySpawner = clock.restart();

	}
	
}

void Game::sSpawnAfterKill(int fragments, Vec2<float> pos_, const sf::Color color_)
{
	float plusAngle = 360.f / (float)(fragments);
	float totalAngle = 0.f;

	//Spawn new entities
	for (int count{ fragments }; count > 0; count--)
	{

		auto e = m_entities.addEntity("SmallEnemy");

		e->cTransform = std::make_shared<CTransform>(Vec2<float>{pos_}, Vec2<float>{sin(totalAngle)*2.f, cos(totalAngle) * 2.f},
			Vec2<float>{0.5f, 0.5f}, ((360.f / (float)(fragments)) / 50.f));

		e->cShape = std::make_shared<CShape>(25.f / 2.f, fragments, Vec2<float>{e->cTransform->pos}, color_, sf::Color::White, 2.f);

		e->cLifeSpan = std::make_shared<CLifespan>(1.f);
		e->cScore = std::make_shared<CScore>(e->cShape->shape.getPointCount() * 200);

		totalAngle += plusAngle;

	};


}

void Game::sMovement()
{
	// TODO create new function that takes tag as param 

	// move objects 
	for (auto& e : m_entities.getEntities("Enemy")) {

		e->cShape->shape.move(e->cTransform->speed.getCords().first, e->cTransform->speed.getCords().second);
		e->cTransform->pos = Vec2{ e->cShape->shape.getPosition().x, e->cShape->shape.getPosition().y };

	}

	for (auto& e : m_entities.getEntities("SmallEnemy")) {

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

		if (e->cShape != nullptr) {
			if (e->cTransform->rotation != 0) {
				e->cShape->shape.rotate(e->cTransform->rotation);
			}
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
			player->cInput->notMoved = false;
		}
		else {
			player->cInput->LEFT = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			player->cInput->RIGHT = true;
			player->cInput->notMoved = false;
		}
		else {
			player->cInput->RIGHT = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			player->cInput->UP = true;
			player->cInput->notMoved = false;
		}
		else {
			player->cInput->UP = false;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			player->cInput->DOWN = true;
			player->cInput->notMoved = false;
		}
		else {
			player->cInput->DOWN = false;
		}
		
	}

}

void Game::sCollision()
{
	// TODO add help function

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

	// check box collision bullet to big enemy
	for (auto& bullet : m_entities.getEntities("Bullet")) {

		for (auto& enemy : m_entities.getEntities("Enemy")) {

			if (bullet->cShape->shape.getGlobalBounds().intersects(enemy->cShape->shape.getGlobalBounds())) {

				bullet->destroy();
				enemy->destroy();

				score += enemy->cScore->score;

				sSpawnAfterKill(enemy->cShape->shape.getPointCount(), enemy->cTransform->pos,
					enemy->cShape->shape.getFillColor());

			}
		}
	}

	for (auto& bullet : m_entities.getEntities("Bullet")) {

		for (auto& s_enemy : m_entities.getEntities("SmallEnemy")) {

			if (bullet->cShape->shape.getGlobalBounds().intersects(s_enemy->cShape->shape.getGlobalBounds())) {

				bullet->destroy();
				s_enemy->destroy();

				score += s_enemy->cScore->score;
			}

		}

	}


	//check player enemy collision if so restart game
	for (auto& player : m_entities.getEntities("Player")) {

		if (player->cInput->notMoved) {
			break;
		}

		for (auto& enemy : m_entities.getEntities("Enemy")) {

			if (player->cShape->shape.getGlobalBounds().intersects(enemy->cShape->shape.getGlobalBounds())) {

				enemy->destroy();
				player->destroy();
				restart();
				break;

			}

		}

	}



}

void Game::sShooting()
{
	if (m_entities.getEntities("Bullet").size() >= 3) {
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
			player->cInput->notMoved = false;


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
		// calculate procent of lifespan passed
		float procent =  1 - (((100 * e->cLifeSpan->elapsedTime.asSeconds()) / e->cLifeSpan->totalTime)) / 100;
		
		
		
		sf::Color newColor{ e->cShape->shape.getFillColor() };

		//change color

		if (e->tag() == "Bullet" && newColor.a * procent < 1) {
			newColor.a = 1;
		}
		else {
			newColor.a = (sf::Uint8)(255 * procent);
		}
		
		e->cShape->shape.setFillColor(newColor);
		

		//change outline
		if (e->tag() != "Bullet") {
			float outThick = 3.f * procent;
			e->cShape->shape.setOutlineThickness(outThick);
		}

		if (e->cLifeSpan->elapsedTime.asSeconds() > e->cLifeSpan->totalTime) {
			e->destroy();
		}

	}


}

void Game::sUpdateScore()
{
	text_score.setString(std::to_string(score));
}

void Game::sImGUI()
{
	ImGui::Begin("Game properties");

	// All tabs
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("Tabs", tab_bar_flags)) {

		if (ImGui::BeginTabItem("Systems")) {

			ImGui::Checkbox("Collisions", &m_collisions);


			ImGui::EndTabItem();

		}

		if (ImGui::BeginTabItem("All Ent")) {

			ImGui::EndTabItem();

		}

		if (ImGui::BeginTabItem("Grouped")) {

			ImGui::EndTabItem();

		}



		ImGui::EndTabBar();
	}


	ImGui::End();
}

void Game::restart()
{
	m_window.close();
	m_entities.restart();

	init();
	initPlayer();
	initBackground();

}



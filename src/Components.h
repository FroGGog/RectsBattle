#include <SFML/Graphics.hpp>
#include "Vec2.h"

class CTransform {

public:

	Vec2<float> pos = { 0.0, 0.0 };
	Vec2<float> speed = { 0.0, 0.0 };
	Vec2<float> scale = { 1.0, 1.0 };

	float rotation = 0;

	CTransform(Vec2<float> pos_, Vec2<float> speed_, Vec2<float> scale_, float angle)
		:pos(pos_), speed(speed_), scale(scale_), rotation(angle) {};

};

class CShape {

public:

	sf::CircleShape shape;

	CShape(float radius, int segments, Vec2<float> position, const sf::Color& fill,
		const sf::Color& outline, float thickness) : shape(radius, segments)
	{
		shape.setFillColor(fill);
		shape.setOutlineColor(outline);
		shape.setOutlineThickness(thickness);
		shape.setOrigin(radius, radius);
		shape.setPosition(position.getCords().first, position.getCords().second);
	};

};

class CVertex {

public:

	sf::VertexArray vertex;

	CVertex(const sf::Color& fillColor) {

		vertex = sf::VertexArray{ sf::Quads, 4 };
		vertex[0].color = fillColor;
		vertex[1].color = fillColor;
		vertex[2].color = fillColor;
		vertex[3].color = fillColor;

	}

};


class CLifespan {

public:

	float totalTime;

	sf::Clock timer;
	sf::Time elapsedTime;

	CLifespan(float total) : totalTime(total) { elapsedTime = timer.getElapsedTime(); };

};

class CShooting {

public: 

	sf::Clock timer;
	sf::Time elapsedTime;

	float reloading;
	bool canShoot = true;

	CShooting(float reloadTime) : reloading(reloadTime) { elapsedTime = sf::seconds(5); };

};


class CScore {

public:

	int score = 0;

	CScore(int s) : score(s) {};

};

class CCollision {

public:

	float radius;

	CCollision(float cRad) : radius(cRad) {};

};

class CAbbility {


public:

	sf::Clock timer;
	sf::Time elapsedTime;

	float reloading;

	sf::CircleShape InShape;
	sf::CircleShape OutShape;

	CAbbility(float reload, float InRad, float outRad) : reloading(reload)
	{
		InShape.setRadius(InRad);
		InShape.setFillColor(sf::Color::Black);
		InShape.setOrigin(sf::Vector2f{ InShape.getPosition().x + InShape.getGlobalBounds().width / 2,
			InShape.getPosition().y + InShape.getGlobalBounds().height / 2 });

		OutShape.setRadius(outRad);
		OutShape.setOrigin(sf::Vector2f{ OutShape.getPosition().x + OutShape.getGlobalBounds().width / 2,
			OutShape.getPosition().y + OutShape.getGlobalBounds().height / 2 });
		OutShape.setPosition(sf::Vector2f{ InShape.getPosition().x + InShape.getGlobalBounds().width / 2,
			InShape.getPosition().y + InShape.getGlobalBounds().height / 2});
		OutShape.setFillColor(sf::Color{ 128,128,128, 150});
		

	
	};

};


class CInput {

public:

	bool UP = false;
	bool DOWN = false;
	bool LEFT = false;
	bool RIGHT = false;

	bool notMoved = true;

	CInput() {};

};

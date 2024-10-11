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

class CInput {

public:

	bool UP = false;
	bool DOWN = false;
	bool LEFT = false;
	bool RIGHT = false;

	CInput() {};

};

#pragma once
#include "Components.h"

class Entity {

private:

	const size_t m_id;
	const std::string m_tag = "Default";
	bool is_alive = true;

public:

	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CLifespan> cLifeSpan;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CShooting> cShooting;
	
	Entity(const std::string& t, const size_t i);

	void destroy();
	bool isAlive();

	const std::string& tag();
	const size_t& id();

};
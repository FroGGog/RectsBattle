#include "Entity.h"

//Entity stuff
Entity::Entity(const std::string& t, const size_t i)
	:m_id(i), m_tag(t){}

void Entity::destroy()
{
	is_alive = false;
}

bool Entity::isAlive()
{
	return is_alive;
}

const std::string& Entity::tag()
{
	return m_tag;
}

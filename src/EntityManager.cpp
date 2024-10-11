#include "EntityManager.h"

void EntityManager::deleteEntities()
{

	for (int i{ 0 }; i < m_entities.size(); i++) {

		if (!m_entities[i]->isAlive()) {
			toDeleteVec.push_back(i);
		}

	}

	//go throught all tag, search for dead entities, add their index to deleteMap with
	//given tag
	for (const auto& elem : m_entityMap) {

		for (int i{ 0 }; i < elem.second.size(); i++) {

			if (!elem.second[i]->isAlive()) {
				toDeleteMap[elem.first].push_back(i);
			}

		}

	}


}

void EntityManager::update()
{

	for (auto& e : m_toAdd) {

		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);

	}
	m_toAdd.clear();

	if (m_entities.size() == 0) {
		return;
	}

	int missing = 0;
	//delete dead entities from all entites
	for (auto index : toDeleteVec) {

		m_entities.erase((m_entities.begin() + index + missing));
		missing--;

	}
	//delete dead entities from tag map of entitites
	for (auto& elem : toDeleteMap) {

		for (auto& index : elem.second) {

			m_entityMap[elem.first].erase(m_entityMap[elem.first].begin() + index);

		}

	}

	toDeleteVec.clear();
	toDeleteMap.clear();
	
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto e = std::make_shared<Entity>(tag, m_totalEntities++);
	m_toAdd.push_back(e);
	return e;
}

EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

void EntityManager::restart()
{
	m_entities.clear();
	m_entityMap.clear();

	m_toAdd.clear();
	toDeleteVec.clear();
	toDeleteMap.clear();

	m_totalEntities = 0;

}

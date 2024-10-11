#include <vector>
#include <map>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager {

private: 

	EntityVec m_entities;
	EntityVec m_toAdd;
	EntityMap m_entityMap;

	std::vector<int> toDeleteVec;
	std::map<std::string, std::vector<int>> toDeleteMap;

	size_t m_totalEntities = 0;

public:

	void update();

	void deleteEntities();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);

	void restart();

};
#include "family.h"
#include "family_binding.h"

using namespace Halley;

Family::Family(FamilyMaskType mask) 
	: inclusionMask(mask)
{}

void Family::addOnEntitiesAdded(FamilyBindingBase* bind)
{
	addEntityCallbacks.push_back(bind);
}

void Family::removeOnEntityAdded(FamilyBindingBase* bind)
{
	addEntityCallbacks.erase(std::remove(addEntityCallbacks.begin(), addEntityCallbacks.end(), bind), addEntityCallbacks.end());
}

void Family::addOnEntitiesRemoved(FamilyBindingBase* bind)
{
	removeEntityCallbacks.push_back(bind);
}

void Family::removeOnEntityRemoved(FamilyBindingBase* bind)
{
	removeEntityCallbacks.erase(std::remove(removeEntityCallbacks.begin(), removeEntityCallbacks.end(), bind), removeEntityCallbacks.end());
}

void Family::notifyAdd(void* entities, size_t count)
{
	for (auto& c: addEntityCallbacks) {
		c->onEntitiesAdded(entities, count);
	}
}

void Family::notifyRemove(void* entities, size_t count)
{
	for (auto& c: removeEntityCallbacks) {
		c->onEntitiesRemoved(entities, count);
	}
}

void Family::removeEntity(Entity& entity)
{
	toRemove.push_back(entity.getEntityId());
}

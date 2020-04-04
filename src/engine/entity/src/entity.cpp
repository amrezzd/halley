#include <halley/data_structures/memory_pool.h>
#include "entity.h"
#include "world.h"

using namespace Halley;

Entity::Entity() = default;
Entity::~Entity() = default;

void Entity::destroyComponents(ComponentDeleterTable& table)
{
	for (auto& component : components) {
		deleteComponent(component.second, component.first, table);
	}
	components.clear();
	liveComponents = 0;
}

void Entity::addComponent(Component* component, int id)
{
	// Put it at the back of the list...
	components.push_back(std::pair<int, Component*>(id, component));

	// ...if there's dead components, swap with the first dead component...
	if (liveComponents < int(components.size())) {
		std::swap(components[liveComponents], components.back());
	}

	// ...and increase the list, therefore putting it in living component territory
	++liveComponents;
}

void Entity::removeComponentAt(int i)
{
	// Put it at the end of the list of living components... (guaranteed to swap with living component)
	std::swap(components[i], components[size_t(liveComponents) - 1]);

	// ...then shrink that list, therefore moving it into dead component territory
	--liveComponents;
}

void Entity::removeAllComponents(World& world)
{
	liveComponents = 0;
	markDirty(world);
}

void Entity::deleteComponent(Component* component, int id, ComponentDeleterTable& table)
{
	TypeDeleterBase* deleter = table.get(id);
	deleter->callDestructor(component);
	PoolPool::getPool(deleter->getSize())->free(component);
}

void Entity::keepOnlyComponentsWithIds(const std::vector<int>& ids, World& world)
{
	auto newEnd = std::remove_if(components.begin(), components.begin() + liveComponents, [&] (const std::pair<int, Component*>& c)
	{
		return std::find(ids.begin(), ids.end(), c.first) == ids.end();
	});
	liveComponents = int(newEnd - components.begin());
	markDirty(world);
}

void Entity::onReady()
{
}

void Entity::markDirty(World& world)
{
	if (!dirty) {
		dirty = true;
		world.onEntityDirty();
	}
}

ComponentDeleterTable& Entity::getComponentDeleterTable(World& world)
{
	return world.getComponentDeleterTable();
}

FamilyMaskType Entity::getMask() const
{
	return mask;
}

void Entity::refresh(MaskStorage& storage, ComponentDeleterTable& table)
{
	if (dirty) {
		dirty = false;

		// Delete stale components
		for (int i = liveComponents; i < int(components.size()); ++i) {
			deleteComponent(components[i].second, components[i].first, table);
		}
		components.resize(liveComponents);

		// Re-generate mask
		auto m = FamilyMask::RealType();
		for (auto i : components) {
			FamilyMask::setBit(m, i.first);
		}
		mask = FamilyMask::getHandle(m, storage);
	}
}

EntityId Entity::getEntityId() const
{
	if (!uid.isValid()) {
		//throw Exception("Entity ID not yet assigned - are you using this before it's spawned?");
	}
	return uid;
}

void Entity::destroy()
{
	alive = false;
	dirty = true;
}

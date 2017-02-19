#include "Entity/EntityManager.h"
#include "Entity/Component.h"

#include <array>
#include <vector>
#include <assert.h>

#include <stdio.h>

namespace Enjon {

	//---------------------------------------------------------------
	EntityHandle::EntityHandle()
	: mID(MAX_ENTITIES), 
	  mState(EntityState::INACTIVE), 
	  mComponentMask(Enjon::ComponentBitset(0)),
	  mManager(nullptr)
	{
	}

	//---------------------------------------------------------------
	EntityHandle::EntityHandle(EntityManager* manager)
	: mID(MAX_ENTITIES), 
	  mState(EntityState::INACTIVE), 
	  mComponentMask(Enjon::ComponentBitset(0)),
	  mManager(manager)
	{
	}

	// TODO(John): Take care of removing all attached components here
	//---------------------------------------------------------------
	EntityHandle::~EntityHandle()
	{
		mManager->Destroy(this);		
	}

	//---------------------------------------------------------------
	void EntityHandle::Reset()
	{
		mID = MAX_ENTITIES;
		mState = EntityState::INACTIVE;
		mComponentMask = Enjon::ComponentBitset(0);
		mManager = nullptr;
		mComponents.clear();
	}

	//---------------------------------------------------------------
	void EntityHandle::SetID(u32 id)
	{
		mID = id;
	}

	//---------------------------------------------------------------
	void EntityHandle::SetPosition(EM::Vec3& position)
	{
		mTransform.SetPosition(position);
	}

	//---------------------------------------------------------------
	void EntityHandle::SetScale(EM::Vec3& scale)
	{
		mTransform.SetScale(scale);
	}

	//---------------------------------------------------------------
	void EntityHandle::SetOrientation(EM::Quaternion& orientation)
	{
		mTransform.SetOrientation(orientation);
	}

	//---------------------------------------------------------------
	void EntityHandle::SetParent(EntityHandle* parent)
	{
		mParent = parent;
	}

	//---------------------------------------------------------------
	EntityManager::EntityManager()
	{
		for (auto i = 0; i < mComponents.size(); i++)
		{
			mComponents.at(i) = nullptr;
		}

		mNextAvailableID = 0;
		mEntities = new std::array<EntityHandle, MAX_ENTITIES>;
	}

	//---------------------------------------------------------------
	EntityManager::~EntityManager()
	{
		// Detach all components from entities
		for (u32 i = 0; i < MAX_ENTITIES; ++i)
		{
			Destroy(&mEntities->at(i));	
		}
		delete[] mEntities;

		// Deallocate all components
		for (u32 i = 0; i < mComponents.size(); ++i)
		{
			delete mComponents.at(i);
			mComponents.at(i) = nullptr;
		}
	}

	//---------------------------------------------------------------
	u32 EntityManager::FindNextAvailableID()
	{
		// Iterate from current available id to MAX_ENTITIES
		for (u32 i = mNextAvailableID; i < MAX_ENTITIES; ++i)
		{
			if (mEntities->at(i).mState == EntityState::INACTIVE)
			{
				mNextAvailableID = i;
				return mNextAvailableID;
			}
		}

		// Iterate from 0 to mNextAvailableID
		for (u32 i = 0; i < mNextAvailableID; ++i)
		{
			if (mEntities->at(i).mState == EntityState::INACTIVE)
			{
				mNextAvailableID = i;
				return mNextAvailableID;
			}
		}

		// Other wise return MAX_ENTITIES, since there are no entity slots left
		return MAX_ENTITIES;
	}

	//---------------------------------------------------------------
	EntityHandle* EntityManager::Allocate()
	{
		// Grab next available id and assert that it's valid
		u32 id = FindNextAvailableID();
		assert(id < MAX_ENTITIES);

		// Find entity in array and set values
		EntityHandle* entity = &mEntities->at(id);
		entity->mID = id;				
		entity->mState = EntityState::ACTIVE; 
		entity->mManager = this;

		// Push back live entity into active entity vector
		mActiveEntities.push_back(entity);

		// Return entity handle
		return entity;
	}

	//---------------------------------------------------------------
	void EntityManager::Destroy(EntityHandle* entity)
	{
		// Assert that entity isn't already null
		assert(entity != nullptr);

		// Push for deferred removal from active entities
		mMarkedForDestruction.push_back(entity);

		// Iterate through entity component list and detach
		for (auto& c : entity->mComponents)
		{
			// Detach(c);
			c->Destroy();
		}	

		// Set state to inactive
		entity->mState = EntityState::INACTIVE;

		// Set bitmask to 0
		entity->Reset();
	}

	//--------------------------------------------------------------
	void EntityManager::Cleanup()
	{
		// Move through dirty list and remove from active entities
		for (auto& c : mMarkedForDestruction)
		{
			// Remove from active entities
			mActiveEntities.erase(std::remove(mActiveEntities.begin(), mActiveEntities.end(), c), mActiveEntities.end());
		}

		mMarkedForDestruction.clear();
	}

	//--------------------------------------------------------------
	void EntityManager::Update(f32 dt)
	{
		// Clean any entities that were marked for destruction
		Cleanup();
	} 
}



















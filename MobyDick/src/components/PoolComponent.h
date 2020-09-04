#pragma once
#include "Component.h"
#include "../ObjectPoolManager.h"

class PoolComponent : public Component
{

public:
	PoolComponent();
	PoolComponent(Json::Value definitionJSON);
	~PoolComponent();

	void update() override;

	bool isAvailable() { return m_isAvailable; }
	void setAvailable(bool available) { m_isAvailable = available; }
	bool isActive() { return m_isActive; }
	void reset();

private:

	//ObjectPoolManager& objectPoolManager; // make this a static reference that only this compoennt can access
	bool m_isAvailable;
	bool m_isActive;
	std::string m_poolId;


};


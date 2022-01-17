#pragma once
#include "Component.h"

class PoolComponent : public Component
{

public:
	PoolComponent(Json::Value componentJSON);
	~PoolComponent();

	void update() override;

	bool isAvailable() { return m_isAvailable; }
	void setAvailable(bool available) { m_isAvailable = available; }
	bool isActive() { return m_isActive; }
	void reset();

private:

	bool m_isAvailable;
	bool m_isActive;
	std::string m_poolId;


};


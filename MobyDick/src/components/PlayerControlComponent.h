#ifndef PLAYER_CONTROL_COMPONENT_H
#define PLAYER_CONTROL_COMPONENT_H

#include <memory>
#include <string>
#include <bitset>

#include "Component.h"

class GameObject;


class PlayerControlComponent : public Component
{
public:

	PlayerControlComponent();
	PlayerControlComponent(std::string gameObjectId, std::shared_ptr<GameObject> parentGameObject);

	~PlayerControlComponent();

	void update() override;


private:
	std::bitset<8> m_controls;

	void init();



};


#endif

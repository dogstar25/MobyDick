#ifndef ATTACHMENTS_COMPONENT_H
#define ATTACHMENTS_COMPONENT_H

#include <vector>
#include <memory>
#include <optional>

#include <json/json.h>
#include "Component.h"
//#include "../GameObject.h"

class Scene;
class GameObject;

struct Attachment
{
	bool addToInventory{ false };
	b2JointType attachB2JointType{ b2JointType::e_weldJoint };
	b2Vec2 attachLocation{};
	std::shared_ptr<GameObject> gameObject;

	Attachment(bool addToInventory, b2JointType attachB2JointType, b2Vec2 attachLocation, std::shared_ptr<GameObject> gameObject) :
		addToInventory(addToInventory),
		attachB2JointType(attachB2JointType),
		attachLocation(attachLocation),
		gameObject(gameObject) {}
};

class AttachmentsComponent :   public Component
{
public:
	AttachmentsComponent();
	AttachmentsComponent(Json::Value componentJSON, Scene* parentScene);
	~AttachmentsComponent();

	void update() override;
	void render();
	void postInit() override;
	void setParent(GameObject* parentObject) override;

	const std::optional<Attachment> getAttachment(std::string id);


private:
	std::vector<Attachment> m_attachments;

};

#endif

